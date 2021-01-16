#ifndef _IMAGE
#define _IMAGE

#include <iostream>
#include <string>

#include "Array.h"
#include "Vec3.h"
#include "ppm.h"

using namespace math;
using namespace imaging;

typedef Vec3<float> Color;

class Image : public Array<Color>{
public:
	// Constructors
	Image() :Array() {}
	Image(const Image &image) :Array(image) {}
	Image(unsigned int width, unsigned int height) :Array(width, height) {}
	Image(unsigned int width, unsigned int height, const vector<Color> data_ptr) :Array(width, height, data_ptr) {}


	bool load(const string & filename, const string & format) {

		if (width != 0 || height != 0 || !buffer.empty()) {
			width = 0;
			height = 0;
			buffer.clear();
		}

		int *w = (int*)&width;
		int *h = (int*)&height;
		const char* charFilename = filename.c_str();

		if (_strcmpi(format.c_str(), ".ppm") == 0) {
			float * readArray = ReadPPM(charFilename, w, h);

			/* Float array should be converted to Color array
			** A set of 3 floats form a Color object
			*/
			Color * colorArray = new Color[*w*(*h)];
			int j = 0;
			for (int i = 0; i < *w*(*h); i++) {
				Color pixelColor = Color(readArray[j], readArray[j + 1], readArray[j + 2]);
				colorArray[i] = pixelColor;
				j += 3;
			}

			// Set width and height and data of the image
			if (*w != 0 && *h != 0 && readArray != nullptr) {
				width = *w;
				height = *h;
				buffer.reserve(width*height);
				for (int i = 0; i < width*height; i++) {
					buffer.push_back(colorArray[i]);
				}
				return true;
			}
			else {
				cout << "No height or width were registered." << endl;
				return false;
			}
		}
		else {
			printf("ERROR: Invalid format. Only .ppm format allowed.\n");
			return false;
		}
	}

	bool save(const string & filename, const string & format) {

		const char* charFilename = filename.c_str();
		// Check if initialized
		if (width == 0 || height == 0 || buffer.empty()) {
			cout << "ERROR: Image " << filename << " is not initilized." << endl;
			return false;
		}
		// Check format
		if (_strcmpi(format.c_str(), ".ppm") == 0) {

			// Color buffer is decoded again to a float array
			float *floatArray = new float[width*height * 3];

			int j = 0;
			for (size_t i = 0; i < width*height; i++) {
				floatArray[j] = buffer[i].r;
				floatArray[j + 1] = buffer[i].g;
				floatArray[j + 2] = buffer[i].b;
				j += 3;
			}

			bool write = WritePPM(floatArray, width, height, charFilename);
			delete[] floatArray;
			return write;
		}
	}
};

#endif
