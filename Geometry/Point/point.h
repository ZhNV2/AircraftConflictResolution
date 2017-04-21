//
// Created by Нико on 21.04.2017.
//

#ifndef ACR_POINT_H
#define ACR_POINT_H

#include "../Vector/vector.h"
class Vector;

class Point {
    public:
        double x, y, z;

        Point();
        Point(double, double, double);
        Point AddVector(const Vector&);
};

#endif //ACR_POINT_H
