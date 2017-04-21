//
// Created by Нико on 21.04.2017.
//

#ifndef ACR_TRACK_H
#define ACR_TRACK_H

#include <vector>
#include "../Point/point.h"

class Track {
private:
    std::vector<Point> track_points;

public:
    Track();
    void AddPoint(Point);
    Point GetTrackPoint(size_t);
    size_t GetLen();
};


class TrackFactory {
public:
    Track *GenerateTrack();
};

#endif //ACR_TRACK_H
