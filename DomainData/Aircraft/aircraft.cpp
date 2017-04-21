//
// Created by Нико on 21.04.2017.
//

#include "aircraft.h"
#include <iostream>

Aircraft::Aircraft(Track *track1, double velocity1) : track(track1), pos_in_track(0),
                                                      time_from_point(0), velocity(velocity1 / 60 / 60),
                                                      is_flying(true), position(track1->GetTrackPoint(0)) {}

void Aircraft::Update(double time) {
    time += time_from_point;
    for (; pos_in_track + 1 < track->GetLen(); pos_in_track++) {
        Point cur_point = track->GetTrackPoint(pos_in_track);
        Point next_point = track->GetTrackPoint(pos_in_track + 1);
        double dist = Vector(cur_point, next_point).Len();
        double time_to_fly = dist / velocity;
        if (time_to_fly > time) {
            break;
        } else {
            time -= time_to_fly;
        }
    }

    if (pos_in_track + 1 == track->GetLen()) {
        is_flying = false;
        return;
    }

    time_from_point = time;
    Point cur_point = track->GetTrackPoint(pos_in_track);
    Point next_point = track->GetTrackPoint(pos_in_track + 1);
    Vector v = Vector(cur_point, next_point);
    v *= (time_from_point * velocity / v.Len());
    position = cur_point.AddVector(v);
    std::cout << position.x << " " << position.y << " " << position.z << std::endl;
}

Point Aircraft::GetPosition() {
    return position;
}

bool Aircraft::IsFlying() {
    return is_flying;
}

Aircraft::~Aircraft() {

}

Track Aircraft::GetPrediction(double time) {
    Track new_track = Track();
    new_track.AddPoint(position);
    for (size_t i = pos_in_track + 1; i < track->GetLen(); i++) {
        Point cur_point = track->GetTrackPoint(i);
        Point prev_point = i == pos_in_track + 1 ? position : track->GetTrackPoint(i - 1);
        double time_to_fly = Vector(prev_point, cur_point).Len() / velocity;
        if (time > time_to_fly - EPS) {
            new_track.AddPoint(cur_point);
            time -= time_to_fly;
        } else {
            new_track.AddPoint(prev_point.AddVector(Vector(prev_point, cur_point) *= (time / time_to_fly)));
            break;
        }
    }
    return new_track;
}

double Aircraft::GetVelocity() {
    return velocity;
}

