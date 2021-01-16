#include <iostream>
#include <vector>
#include <string>

#include "Array.h"
#include "Filter.h"
#include "Image.h"
#include "Vec3.h"

using namespace std;
using namespace math;


int main(int argc, char* argv[]) {

	
	cout << endl << "This program applies filters given to an image." << endl;
	if (argc > 2) {
		// File name is always last parameter
		string imageName = argv[argc-1];
		
		if (imageName.length() > 4) { // Room for format '.ppm'

			string format(imageName.end() - 4, imageName.end());
			Image img;

			// Call load image
			bool loadImage = img.load(imageName, format);
			if (loadImage) {
				// Print Image Info
				cout << "----------------------------" << endl;
				cout << "Filename: " << imageName << endl;
				cout << "Width:    " << img.getWidth() << endl;
				cout << "Height:   " << img.getHeight() << endl;
				cout << "----------------------------" << endl;

				string newImageName = "filtered_";

				// Save new Image Name
				size_t found = imageName.find_last_of("\\");
				if (found > 0)newImageName = newImageName.append(imageName.substr(found + 1));
				else newImageName = newImageName.append(imageName);
				
				// Parameters should be of form : 
				// program -f [linear] [parameters] -f[][]... imageName.ppm
				int i = 2;
				bool check = true;
				Image newImage;
				
				// Check parameter "filter"
				while (i < argc-1) {
					if (_strcmpi(argv[i], "linear") == 0 && _strcmpi(argv[i-1], "-f") == 0) { // Check if next filter is LINEAR
						
						FilterLinear fl(img);
						// Linear filter needs 6 parameters
						if (argc - i > 6) {
							// Pass all parameters for linear filter in vector
							i++;
							for (int j = 0; j < 6; j++) {
								fl.setVector(atof(argv[i]));
								i++;
							}	
							// Call Linear Filter
							newImage = fl << img;
							i++;
						}
						else {
							printf("ERROR: Not enough parameters for linear filter.\n");
							check = false;
							break;
						}
					}
					else if (_strcmpi(argv[i], "gamma") == 0 && _strcmpi(argv[i-1], "-f") == 0) { // Check if next filter is GAMMA
						FilterGamma fg(img);
						// Gamma filter needs 1 parameter
						if (argc - i > 1) {
							// Pass all parameters for gamma filter in vector
							i++;
							float gamma = atof(argv[i]);

							if (gamma >= 0.5 && gamma <= 2.0) {

								fg.setGamma(gamma);

								// Call Gamma Filter
								newImage = fg << img;
								i = i + 2;
							}
							else {
								printf("ERROR: Incorrect value for gamma.\n");
								check = false;
								break;
							}	
						}
						else {
							printf("ERROR: Not enough parameters for gamma filter.\n");
							check = false;
							break;
						}
					}
					else {
						printf("ERROR: Wrong parameters.\n");
						check = false;
						break;
					}
					img = newImage;
				}
				// SAVE IMAGE
				if (check) {
					bool save = img.save(newImageName, ".ppm");
					if (save) {
						// Print Image Info
						cout << "----------------------------" << endl;
						cout << "Filename: " << newImageName << endl;
						cout << "Width:    " << img.getWidth() << endl;
						cout << "Height:   " << img.getHeight() << endl;
						cout << "----------------------------" << endl;
					}
				}
			}
			else {
				printf("ERROR: Image could not load.\n");
			}
		}
		else {
			printf("ERROR: Incorrect image name.\n");
		}
	}
	else {
		printf("ERROR: No image or filters were given.\n");
	}
	
	
	system("PAUSE");
	return 0;
}