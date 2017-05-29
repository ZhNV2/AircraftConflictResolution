#pragma once

#include "../Point/point.h"

struct Segment {
	Point first_point, second_point;
	Segment(Point, Point);
};