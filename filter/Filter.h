#ifndef _FILTER
#define _FILTER

#include <iostream>
#include <string>
#include <vector>
#include <math.h>

#include "Image.h"

using namespace std;
using namespace math;

class Filter {
public:
	Filter() {}
	Filter(Filter & src): Filter() {}
	~Filter() {}

	// Operator overload <<
	virtual Image operator << (const Image & image) = 0;
	
};

class FilterLinear : public Filter { // p'(x,y) = a * p(x,y) + c 
private:
	vector<float> parameters; // parameters from user a and c
	Color A;
	Color C;
	Image filteredImage;
public:
	FilterLinear(Image img) :A ({}), C({}),filteredImage(img.getWidth(),img.getHeight()) {}
	
	void setVector(float c) {
		parameters.push_back(c);		
	}

	void setA() {
		A = Vec3<float>(parameters[0], parameters[1], parameters[2]);
	}
	void setC() {
		C = Vec3<float>(parameters[3], parameters[4], parameters[5]);
	}

	Image operator << (const Image & image) {
		setA();
		setC();
		for (int y = 0; y < filteredImage.getHeight(); y++) {
			for (int x = 0; x < filteredImage.getWidth(); x++) {
				Color normal = image.getPixel(x, y);
				Color filtered = A * normal + C;
				
				// Check boundaries
				filtered = filtered.clampToLowerBound(0);
				filtered = filtered.clampToUpperBound(1);

				filteredImage.setPixel(x, y, filtered);
			}
		}
		parameters.clear();
		return filteredImage;
	}
};
class FilterGamma : public Filter { // p'(x,y) = p(x,y)^gamma
private:
	float gamma;
	Image filteredImage;
public:
	FilterGamma(Image image): gamma(1.0),filteredImage(image.getWidth(),image.getHeight()){}
	
	void setGamma(float g) {
		gamma = g;
	}
	// Gamma filter formula for each pixel
	Image operator << (const Image & image) {
		for (int y = 0; y < filteredImage.getHeight(); y++) {
			for (int x = 0; x < filteredImage.getWidth(); x++) {
				Color normal = image.getPixel(x, y);
				float redChannel = pow(normal.r, gamma);
				float greenChannel = pow(normal.g, gamma);
				float blueChannel = pow(normal.b, gamma);
				Color filtered = Color(redChannel, greenChannel, blueChannel);

				filteredImage.setPixel(x, y, filtered);
			}
		}
		return filteredImage;;
	}
};


#endif