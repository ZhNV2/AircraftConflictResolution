//
// Created by Нико on 21.04.2017.
//

#include <cmath>
#include "vector.h"

Vector::Vector(const Point &p1, const Point &p2) {
    x = p2.x - p1.x;
    y = p2.y - p1.y;
    z = p2.z - p1.z;
}

Vector::Vector(double x1, double y1, double z1) : x(x1), y(y1), z(z1) {
}

double Vector::Len() const {
    return sqrt(x * x + y * y + z * z);
}

Vector &Vector::operator*=(double k) {
    x *= k;
    y *= k;
    z *= k;
    return *this;
}

double Vector::cos(const Vector & v1, const Vector & v2) {
	return (v1.x * v2.x + v1.y * v2.y) / v1.Len() / v2.Len();
}
