//
// Created by Нико on 21.04.2017.
//

#include <cmath>
#include <iostream>
#include "conflict_manager.h"
#include <algorithm>


namespace {

    const double PREDICTION_TIME = 5; // s
    const double MIN_DIST = 1;
    const double EPS = 1e-9;

    double DetectConflict(Point p1, Point p2, Vector v1, Vector v2, double max_time) {
        if (Vector(p1, p2).Len() < MIN_DIST) return 0;
        double x0 = p1.x - p2.x, y0 = p1.y - p2.y, z0 = p1.z - p2.z;
        double dx = v1.x - v2.x, dy = v1.y - v2.y, dz = v1.z - v2.z;
        double A = dx * dx + dy * dy + dz * dz;
        double B = 2 * x0 * dx + 2 * y0 * dy + 2 * z0 * dz;
        double C = x0 * x0 + y0 * y0 + z0 * z0 - MIN_DIST * MIN_DIST;
        double D = B * B - 4 * A * C;
        if (D < 0 || A < EPS) return -1;
        D = sqrt(D);
        double x1 = (-B - D) / 2 / A, x2 = (-B + D) / 2 / A;
        if (x1 > x2) std::swap(x1, x2);
        if (-EPS < x1 && x1 < max_time + EPS) return x1;
        if (-EPS < x2 && x2 < max_time + EPS) return x2;
        return -1;
    }


	void ProcessTwoAircrafts(Aircraft_ptr aircraft1, Aircraft_ptr aircraft2, 
		const Track &track1, const Track &track2, std::vector<ConflictHolder> &conflicts) {
		

	}
}

double ConflictManager::CheckIntersection(const Track &track1, const Track &track2, double velocity1, double velocity2) {
	double time = 0;
	size_t pos_in_track_1 = 0, pos_in_track_2 = 0;
	if (track1.GetLen() == 0 || track2.GetLen() == 0) return -2;
	Point pos1 = track1.GetTrackPoint(0);
	Point pos2 = track2.GetTrackPoint(0);

	while (pos_in_track_1 + 1 < track1.GetLen() &&
		pos_in_track_2 + 1 < track2.GetLen()) {
		Vector v1 = Vector(pos1, track1.GetTrackPoint(pos_in_track_1 + 1));
		Vector v2 = Vector(pos2, track2.GetTrackPoint(pos_in_track_2 + 1));
		double time1 = v1.Len() / velocity1, time2 = v2.Len() / velocity2;
		double fly_time = std::min(time1, time2);
		Vector norm_v1 = v1;
		norm_v1 *= (velocity1 / v1.Len());
		Vector norm_v2 = v2;
		norm_v2 *= (velocity2 / v2.Len());
		double conflict_time = DetectConflict(pos1, pos2, norm_v1, norm_v2, fly_time);
		if (conflict_time < 0) {
			if (time1 < time2) {
				pos1 = track1.GetTrackPoint(++pos_in_track_1);
				v2 *= (fly_time / time2);
				pos2 = pos2.AddVector(v2);
			}
			else {
				pos2 = track2.GetTrackPoint(++pos_in_track_2);
				v1 *= (fly_time / time1);
				pos1 = pos1.AddVector(v1);
			}
			time += fly_time;
		}
		else {
			std::cout << "conflict detected , time = " << (time + conflict_time) << std::endl;
			return time + conflict_time;
		}
	}
	return -2;
}

ConflictManager::ConflictManager(std::vector<Aircraft_ptr> v) : aircrafts(v) {}

double ConflictManager::GetPredictionTime() {
	return PREDICTION_TIME;
}

std::vector<ConflictHolder>& ConflictManager::GetConflicts() {
	return conflicts;
}

std::vector<Aircraft_ptr>& ConflictManager::GetAircrafts() {
	return aircrafts;
}

void ConflictManager::Update(double) {
    conflicts.clear();
    std::vector<Track> predictions;
	for (auto aircraft : aircrafts) {
		auto prediction = aircraft->GetPrediction(PREDICTION_TIME);
        predictions.push_back(prediction);
    }
    for (size_t i = 0; i < aircrafts.size(); i++) {
        for (size_t j = i + 1; j < aircrafts.size(); j++) {
			if (!aircrafts[i]->IsFlying()) return;
			if (!aircrafts[j]->IsFlying()) return;
			double conflict_time = CheckIntersection(predictions[i], predictions[j], aircrafts[i]->GetVelocity(), aircrafts[j]->GetVelocity());
			if (conflict_time > -1) {
				if (conflict_time < 0.01) {
					std::cerr << "CONFLICT!!!!!!!\n";
				}
				conflicts.push_back(ConflictHolder(aircrafts[i], aircrafts[j], conflict_time));
			}
        }
    }
}

ConflictManager::~ConflictManager() {
}

ConflictHolder::ConflictHolder(Aircraft_ptr first_aircraft, Aircraft_ptr second_aircraft, double time_to_conflict)
        : first_aircraft(first_aircraft), second_aircraft(second_aircraft), time_to_conflict(time_to_conflict) {}
