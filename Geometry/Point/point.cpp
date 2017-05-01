//
// Created by Нико on 21.04.2017.
//

#include <cmath>
#include "point.h"

Point::Point() : x(0), y(0), z(0) {}

Point::Point(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}


Point Point::AddVector(const Vector &v) {
    return Point(x + v.x, y + v.y, z + v.z);
}

bool Point::operator==(const Point &p) {
	double EPS = 1e-9;
	return abs(x - p.x) < EPS && abs(y - p.y) < EPS && abs(z - p.z) < EPS;
}

bool Point::operator!=(const Point &p) {
	return !(*this == p);
}

