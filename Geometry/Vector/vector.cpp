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

double Vector::Len() {
    return sqrt(x * x + y * y + z * z);
}

Vector &Vector::operator*=(double k) {
    x *= k;
    y *= k;
    z *= k;
    return *this;
}
