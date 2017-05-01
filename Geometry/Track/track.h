//
// Created by Нико on 21.04.2017.
//

#ifndef ACR_TRACK_H
#define ACR_TRACK_H

#include <vector>
#include <memory>
#include "../Point/point.h"

class Track {
private:
    std::vector<Point> track_points;
	void Unique();

public:
    Track();
	std::pair<Track, size_t> GetSlice(double begin, double end, double velocity) const;
	void InsertPoint(Point, size_t pos);
	void InsertTrack(size_t, Point, const Track&);
    void AddPoint(Point);
    Point GetTrackPoint(size_t) const;
    size_t GetLen() const;
};

typedef std::shared_ptr<Track> Track_ptr;



class TrackFactory {
public:
    Track_ptr GenerateTrack();
};

#endif //ACR_TRACK_H
