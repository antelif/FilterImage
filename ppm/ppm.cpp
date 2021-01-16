#include <iostream>
#include <fstream>
#include <string>

#include "ppm.h"
using namespace std;

namespace imaging
{
	/*! Reads a PPM image file and returns a pointer to a newly allocated float array containing the image data.
	 *
	 * Values in the returned array are in the range [0,1] and for each pixel, three values are stored.
	 *
	 *  \param filename is the null-terminated string of the name of the file to open.
	 *  \param w is the address of an integer parameter to return the width of the image into.
	 *  \param h is the address of an integer parameter to return the height of the image into.
	 *
	 *  \return a pointer to a new float array of 3 X w X h floats, that contains the image data.
	 *  If the reading operation failed the function returns nullptr.
	 */
	float * ReadPPM(const char * filename, int * w, int * h) {

		string type; // P6 expected
		int width;
		int height;
		int pixelValue; // 255 expected

		// Opens file
		ifstream iFile(filename, ios::in | ios::binary);
		if (!iFile.is_open()) {
			printf("ERROR: File not opened for reading.\n");
			return nullptr;
		}

		// Read Header of the file
		iFile >> type >> width >> height >> pixelValue;

		if (type.compare("P6") != 0 || pixelValue != 255) {
			printf("ERROR: File has wrong arguements.\n");
			return nullptr;
		}
		else {
			// Set pointers
			*w = width;
			*h = height;
			// Create float Array
			float *imageReadArray = new float[width*height * 3];
			unsigned char *charArray = new unsigned char[3];

			iFile.seekg(1, ios::cur);
			// Read data
			int j = 0;
			for (int i = 0; i < (*w)*(*h); i++) {
				
				iFile.read((char*)charArray, 3);

				imageReadArray[j] = ((float)charArray[0]) / 255.f;
				imageReadArray[j + 1] = ((float)charArray[1]) / 255.f;
				imageReadArray[j + 2] = ((float)charArray[2]) / 255.f;
				j += 3;
				
			}
			delete [] charArray;
			iFile.close();
			return imageReadArray;
		}
	}

	/*! Writes an image buffer as a PPM file.
	*
	*  \param data contains the image buffer. The data are RGB float tripplets with values in the range [0,1].
	*  \param w is the width of the image in pixels.
	*  \param h is the height of the image in pixels.
	*  \param filename is the null-terminated string of the name of the file to create.
	*
	*  \return true if the image save operation was successful, false otherwise. If the data parameter is nulltr, the
	*  function returns immediately with a false return value.
	*/
	bool WritePPM(const float * data, int w, int h, const char * filename) {

		// Check if variable data is empty
		if (data == nullptr) {
			printf("ERROR: Thre is not date to the buffer of image to be created.\n");
			return false;
		}
		else {
			// Create a new file, open it and write
			fstream oFile(filename, ios::out | ios::binary);
			if (!oFile.is_open()) {
				printf("ERROR: New file could not open.\n");
				return false;
			}
			else {
				oFile << "P6";
				oFile << endl;
				oFile << w;
				oFile << endl;
				oFile << h;
				oFile << endl;
				oFile << "255";
				oFile << endl;

				int j = 0;
				unsigned char* charArray = new unsigned char[3];
				for (int i = 0; i < w*h; i++) {
					
					charArray[0] = (unsigned char)(data[j] * 255.f);
					charArray[1] = (unsigned char)(data[j + 1] * 255.f);
					charArray[2] = (unsigned char)(data[j + 2] * 255.f);

					oFile.write((char*)charArray, 3*sizeof(char));

					j += 3;
				}
				
				delete[] charArray;
				oFile.close();
			}
			return true;
		}
	}

} //namespace imaging

