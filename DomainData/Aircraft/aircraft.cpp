//
// Created by Нико on 21.04.2017.
//

#include "aircraft.h"
#include <iostream>

Aircraft::Aircraft(Track_ptr track1, double velocity1, int ID1) : track(track1), pos_in_track(0),
                                                      time_from_point(0), velocity(velocity1 / 60 / 60),
                                                      is_flying(true), position(track1->GetTrackPoint(0)), ID(ID1), flown(0) {}

void Aircraft::Update(double time) {
	flown += time;
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
	std::cout << "aircraft " << ID << std::endl;
    std::cout << position.x << " " << position.y << " " << position.z << std::endl;
	for (size_t i = 0; i < track->GetLen(); i++) std::cout << "(" << track->GetTrackPoint(i).x << " " << track->GetTrackPoint(i).y << " " << track->GetTrackPoint(i).z << ")";
	std::cout << std::endl;
}

Point Aircraft::GetPosition() const {
    return position;
}

bool Aircraft::IsFlying() const {
    return is_flying;
}

Aircraft::~Aircraft() {

}

Track Aircraft::GetPrediction(double time) {
	if (!is_flying) return Track();
	if (position != track->GetTrackPoint(pos_in_track)) {
		track->InsertPoint(position, pos_in_track + 1);
		pos_in_track++;
	}
	
	time_from_point = 0;
	auto predict = track->GetSlice(flown, flown + time, velocity);
	Track new_track = predict.first;

	track->InsertPoint(new_track.GetTrackPoint(new_track.GetLen() - 1), predict.second);
	last_prediction = new_track;
	return new_track;
}


double Aircraft::GetVelocity() const {
    return velocity;
}

Track Aircraft::GetLastPredicion() const {
	return last_prediction;
}

int Aircraft::GetID() const {
	return ID;
}

void Aircraft::InsertTrack(Point p, const Track &track1) {
	track->InsertTrack(pos_in_track, p, track1);
}

