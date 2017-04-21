//
// Created by Нико on 21.04.2017.
//

#ifndef ACR_VECTOR_H
#define ACR_VECTOR_H

#include "../Point/point.h"

class Point;

class Vector {
public:
    double x, y, z;
    Vector(const Point&, const Point&);
    double Len();
    Vector& operator *= (double k);
};
#endif //ACR_VECTOR_H
