//
// Created by Нико on 21.04.2017.
//

#include "aircraft.h"
#include <iostream>

Aircraft::Aircraft(Track_ptr track1, double velocity1, int ID1) : track_(track1), pos_in_track_(0),
                                                      time_from_point_(0), velocity_(velocity1 / 60 / 60),
                                                      is_flying_(true), position_(track1->GetTrackPoint(0)), ID_(ID1), flown_(0) {}

Track Aircraft::GetTrack() {
	return *track_;
}

void Aircraft::Update(double time) {
	flown_ += time;
    time += time_from_point_;
    for (; pos_in_track_ + 1 < track_->GetLen(); pos_in_track_++) {
        Point cur_point = track_->GetTrackPoint(pos_in_track_);
        Point next_point = track_->GetTrackPoint(pos_in_track_ + 1);
        double dist = Vector(cur_point, next_point).Len();
        double time_to_fly = dist / velocity_;
        if (time_to_fly > time) {
            break;
        } else {
            time -= time_to_fly;
        }
    }

    if (pos_in_track_ + 1 == track_->GetLen()) {
        is_flying_ = false;
        return;
    }

    time_from_point_ = time;
    Point cur_point = track_->GetTrackPoint(pos_in_track_);
    Point next_point = track_->GetTrackPoint(pos_in_track_ + 1);
    Vector v = Vector(cur_point, next_point);
    v *= (time_from_point_ * velocity_ / v.Len());
    position_ = cur_point.AddVector(v);
	std::cout << "aircraft " << ID_ << std::endl;
    std::cout << position_.x << " " << position_.y << " " << position_.z << std::endl;
	for (size_t i = 0; i < track_->GetLen(); i++) std::cout << "(" << track_->GetTrackPoint(i).x << " " << track_->GetTrackPoint(i).y << " " << track_->GetTrackPoint(i).z << ")";
	std::cout << std::endl;
}

Point Aircraft::GetPosition() const {
    return position_;
}

bool Aircraft::IsFlying() const {
    return is_flying_;
}

Track Aircraft::GetPrediction(double time) {
	if (!is_flying_) return Track();
	if (position_ != track_->GetTrackPoint(pos_in_track_)) {
		track_->InsertPoint(position_, pos_in_track_ + 1);
		pos_in_track_++;
	}
	
	time_from_point_ = 0;
	auto predict = track_->GetSlice(flown_, flown_ + time, velocity_);
	Track new_track = predict.first;

	track_->InsertPoint(new_track.GetTrackPoint(new_track.GetLen() - 1), predict.second);
	last_prediction_ = new_track;
	return new_track;
}


double Aircraft::GetVelocity() const {
    return velocity_;
}

Track Aircraft::GetLastPredicion() const {
	return last_prediction_;
}

int Aircraft::GetID() const {
	return ID_;
}

void Aircraft::InsertTrack(Point p, const Track &track1) {
	track_->InsertTrack(pos_in_track_, p, track1);
}

