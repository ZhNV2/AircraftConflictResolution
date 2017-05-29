//
// Created by Нико on 21.04.2017.
//

#include <cstdlib>
#include "track.h"
#include <cassert>

namespace {
	const double EPS = 1e-9;
}

void Track::Unique() {
	std::vector<Point> unique;
	for (auto p : track_points_) {
		if (unique.empty() || unique.back() != p) unique.push_back(p);
	}
	track_points_ = unique;
}

Track::Track() {}



std::pair<Track, size_t> Track::GetSlice(double begin, double end, double velocity) const {
	bool in_slice = false;
	double time = 0;
	Track new_track = Track();
	for (size_t i = 1; i <= GetLen(); i++) {
		Point prev_point = GetTrackPoint(i - 1);
		if (in_slice) {
			new_track.AddPoint(prev_point);
		}
		if (i == GetLen()) {
			return { new_track, GetLen() };
		}
		Point cur_point = GetTrackPoint(i);
		double time_to_fly = Vector(prev_point, cur_point).Len() / velocity;
		if (!in_slice) {
			if (time < begin + EPS && time + time_to_fly > begin) {
				new_track.AddPoint(prev_point.AddVector(Vector(prev_point, cur_point) *= ((begin - time) / time_to_fly)));
				in_slice = true;
			}
		}
		if (in_slice) {
			if (time < end + EPS && time + time_to_fly > end) {
				new_track.AddPoint(prev_point.AddVector(Vector(prev_point, cur_point) *= ((end - time) / time_to_fly)));
				return { new_track, i };
			}
		}
		time += time_to_fly;
	}
	assert(false);
}

void Track::InsertPoint(Point p, size_t pos) {
	track_points_.insert(track_points_.begin() + pos, p);
	Unique();
}

void Track::InsertTrack(size_t begin, Point p, const Track &track) {
	size_t end = begin;
	while (track_points_[end] != p) end++;
	track_points_.erase(track_points_.begin() + begin + 1, track_points_.begin() + end + 1);
	std::vector<Point> to_insert;
	for (int i = 0; i < (int)track.GetLen(); i++) {
		to_insert.push_back(track.GetTrackPoint(i));
	}
	track_points_.insert(track_points_.begin() + begin + 1, to_insert.begin(), to_insert.end());
	Unique();
}

void Track::AddPoint(Point p) {
    track_points_.push_back(p);
}

Point Track::GetTrackPoint(size_t i) const {
    return track_points_.at(i);
}

size_t Track::GetLen() const {
    return track_points_.size();
}

Track_ptr TrackFactory::GenerateTrack() {
    static int x = -1;
    x++;
    Track_ptr track = std::make_shared<Track>(Track());
	if (x == 0) {
		for (int i = 0; i < 10; i++) {
			track->AddPoint(Point(i, i, 0));
		}
	} else if (x == 1) {
		for (int i = 0; i < 10; i++) {
			track->AddPoint(Point(i, 10 - i, 0));
		}
	} else if (x == 2) {
		for (int i = 0; i < 10; i++) {
			track->AddPoint(Point(i, 100, 0));
		}
	} else if (x == 3) {
		for (int i = 0; i < 10; i++) {
			track->AddPoint(Point(10 - i, 100, 0));
		}
	} else if (x == 4) {
		for (int i = 0; i < 10; i++) {
			track->AddPoint(Point(100 + i, 100 + i, 0));
		}
	}
    
    return track;
}
