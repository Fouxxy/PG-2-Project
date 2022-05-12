#ifndef MY_MATH_H_
#define MY_MATH_H_
#define _USE_MATH_DEFINES
#include <cmath>

static class My_Math {
public:
	float static toRadians(float degrees) {
		return degrees * M_PI / 180;
	};
	float static toDegrees(float radians) {
		return radians * 180 / M_PI;
	};
private:
};


#endif