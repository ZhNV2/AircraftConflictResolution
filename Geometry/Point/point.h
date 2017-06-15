#pragma once

#include "../Vector/vector.h"
class Vector;

class Point {
public:
	double x, y, z;

	Point();
	Point(double, double, double);
	Point AddVector(const Vector&);
	bool operator==(const Point&);
	bool operator!=(const Point&);
};

