#ifndef _ARRAY
#define _ARRAY

#include <iostream>
#include <string>
#include <vector>
#include "Vec3.h"

/*! The imaging namespace contains every class or function associated with the image storage, compression and manipulation.
 */
using namespace std;

namespace math {

	template <typename T>
	class Array
	{
	protected:
		// Variables
		vector<T> buffer;                              //! Holds the image data.

		unsigned int width, 						 //! The width of the image (in pixels)
			height;							//! The height of the image (in pixels)

	public:

		// Getters
		const unsigned int getWidth() const { return width; }

		const unsigned int getHeight() const { return height; }

		vector<T> getRawDataPtr() {
			return buffer;
		}

		// Pixel mutators
		T getPixel(unsigned int x, unsigned int y) const {

			if (x < 0 || y > height || y<0 || x>width || buffer.empty()) {
				printf("ERROR: [x,y] out of bounds, pixel set to black.\n");
				T pixelBlack = T();
				return pixelBlack;
			}
			else {
				T pixelT = buffer[y*width + x];
				return pixelT;
			}
		}

		void setPixel(unsigned int x, unsigned int y, T & value) {

			if (x < 0 || y > height || y<0 || x>width) {

				printf("ERROR: Set pixel failure.\n");
				return;
			}
			else {
				buffer.push_back(value);
			}
		}

		// Changes buffer internal data
		void setData(const vector<T> & data_ptr) {
			buffer.clear();
			for (T t : data_ptr) {
				buffer.push_back(t);

			}
		}

		// Constructors
		Array() :width(0), height(0), buffer({}) {}

		Array(unsigned int width, unsigned int height) :width(width), height(height), buffer({}) {}

		Array(unsigned int width, unsigned int height, const vector<T> data_ptr):width(width),height(height) {
			setData(data_ptr);
		}


		Array(const Array &src) {
			Array::width = src.width;
			Array::height = src.height;
			setData(src.buffer);
		}

		// Destructor
		~Array() {}

		Array & operator = (const Array & right) {

			///if (this == &right) {
			//	return *this;
			//}
			Array::width = right.width;
			Array::height = right.height;
			setData(right.buffer);
			return *this;
		}
		
		T & operator ()(int x, int y) {
			// Make necessary checks
			if (x<0 || y<0 || x>width || y> height) {
				return buffer[0];
			}
			else {
				return buffer[y * width + x];
			}
		}
	};
}

#endif