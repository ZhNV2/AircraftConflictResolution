#pragma once

#include "../Point/point.h"

class Point;

class Vector {
public:
    double x, y, z;
    Vector(const Point&, const Point&);
    double Len();
    Vector& operator *= (double k);
};
