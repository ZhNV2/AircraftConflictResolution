//
// Created by Нико on 21.04.2017.
//

#include <algorithm>
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
	if (p == Point(1, 5, 0)) {
		begin = begin;
	}
	track_points_.erase(track_points_.begin() + begin + 1, track_points_.begin() + end );
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



namespace {

	const int T = 10;

	bool in(int x, int y) { return 0 <= x && x < T && 0 <= y && y < T; }

	double randFrom(double l, double r) {
		return l + (r - l) * (rand() % 20) / 20.;
	}

	int dist(int i, int j, int i1, int j1) {
		return abs(i - i1) + abs(j - j1);
	}

	double randFromCell(int i) {
		double D = 20 / T;
		return randFrom(i * D, (i + 1) * D);
	}

	const int DIRS = 8;
	int dx[DIRS] = { 1, 1, 1, 0, 0, -1, -1, 1 };
	int dy[DIRS] = { 0, 1, -1, 1, -1, 0, 1, -1 };
	int p[DIRS] = { 0, 1, 2, 3, 4, 5, 6, 7 };

	void gen(Track_ptr tr, int curi, int curj, int toi, int toj) {
		tr->AddPoint(Point(randFromCell(curi), randFromCell(curj), 0));
		std::random_shuffle(p, p + DIRS);
		for (int t = 0; t < DIRS; t++) {
			if (dist(curi, curj, toi, toj) > dist(curi + dx[p[t]], curj + dy[p[t]], toi, toj)) {
				gen(tr, curi + dx[p[t]], curj + dy[p[t]], toi, toj);
				break;
			}
		}
	}
}

Track_ptr TrackFactory::GenerateTrack() {
	/*
	Track_ptr track = std::make_shared<Track>(Track());;
	gen(track, rand() % T, rand() % T, rand() % T, rand() % T);
	return track;
	*/
    
	
	
	static int x = -1;
    x++;
	int T = 2;
    Track_ptr track = std::make_shared<Track>(Track());
	if (x == 0) {
		for (int i = 0; i < 10; i++) {
			track->AddPoint(Point(T * i, T * i, 0));
		}
	} else if (x == 1) {
		for (int i = 0; i < 10; i++) {
			track->AddPoint(Point(T * i, T * (10 - i), 0));
		}
	} else if (x == 2) {
		for (int i = 0; i < 10; i++) {
			track->AddPoint(Point(T * 5, T * i, 0));
		}
	} else if (x == 3) {
		for (int i = 0; i < 10; i++) {
			track->AddPoint(Point(T * (10 - i), T * 5, 0));
		}
	} 
	return track;
	


    

}
