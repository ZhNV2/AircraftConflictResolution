//
// Created by Нико on 21.04.2017.
//

#include <cstdlib>
#include "track.h"

Track::Track() {}

void Track::AddPoint(Point p) {
    track_points.push_back(p);
}

Point Track::GetTrackPoint(size_t i) {
    return track_points.at(i);
}

size_t Track::GetLen() {
    return track_points.size();
}

Track *TrackFactory::GenerateTrack() {
    static int x = -1;
    x++;
    Track *track = new Track();
    for (int i = 0; i < 10; i++) {
        track->AddPoint(Point(i, x == 1 ? 10 - i : i, 0));
    }
    return track;
}
