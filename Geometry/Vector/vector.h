#pragma once

#include "../Point/point.h"

class Point;

class Vector {
public:
    double x, y, z;
    Vector(const Point&, const Point&);
	Vector(double x1, double y1, double z1);
    double Len() const;
    Vector& operator *= (double k);
	static double cos(const Vector &v1, const Vector &v2);
};
