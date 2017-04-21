//
// Created by Нико on 21.04.2017.
//

#include <cmath>
#include <iostream>
#include "conflict_manager.h"

ConflictManager::ConflictManager(std::vector<Aircraft *> &v) : aircrafts(v) {}

void ConflictManager::Update(double) {
    conflicts.clear();
    std::vector<Track> predictions;
    for (auto aircraft : aircrafts) {
        predictions.push_back(aircraft->GetPrediction(PREDICTION_TIME));
    }

    for (size_t i = 0; i < aircrafts.size(); i++) {
        for (size_t j = i + 1; j < aircrafts.size(); j++) {
            Aircraft *aircraft1 = aircrafts[i];
            Aircraft *aircraft2 = aircrafts[j];
            if (!aircraft1->IsFlying()) continue;
            if (!aircraft2->IsFlying()) continue;
            double time = 0;
            size_t pos_in_track_1 = 0, pos_in_track_2 = 0;

            Point pos1 = predictions[i].GetTrackPoint(0);
            Point pos2 = predictions[j].GetTrackPoint(0);

            while (pos_in_track_1 + 1 < predictions[i].GetLen() &&
                   pos_in_track_2 + 1 < predictions[j].GetLen()) {
                Vector v1 = Vector(pos1, predictions[i].GetTrackPoint(pos_in_track_1 + 1));
                Vector v2 = Vector(pos2, predictions[j].GetTrackPoint(pos_in_track_2 + 1));
                double time1 = v1.Len() / aircraft1->GetVelocity(), time2 = v2.Len() / aircraft2->GetVelocity();
                double fly_time = std::min(time1, time2);
                Vector norm_v1 = v1;
                norm_v1 *= (aircraft1->GetVelocity() / v1.Len());
                Vector norm_v2 = v2;
                norm_v2 *= (aircraft2->GetVelocity() / v2.Len());
                double conflict_time = detectConflict(pos1, pos2, norm_v1, norm_v2, fly_time);
                if (conflict_time < 0) {
                    if (time1 < time2) {
                        pos1 = predictions[i].GetTrackPoint(++pos_in_track_1);
                        v2 *= (fly_time / time2);
                        pos2 = pos2.AddVector(v2);
                    }  else {
                        pos2 = predictions[j].GetTrackPoint(++pos_in_track_2);
                        v1 *= (fly_time / time1);
                        pos1 = pos1.AddVector(v1);
                    }
                    time += fly_time;
                } else {
                    std::cout << "conflict detected , time = " << (time + conflict_time)  << std::endl;
                    conflicts.push_back(ConflictHolder(aircraft1, aircraft2, time + conflict_time));
                    break;
                }
            }
        }
    }

}

double ConflictManager::detectConflict(Point p1, Point p2, Vector v1, Vector v2, double max_time) {
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


ConflictHolder::ConflictHolder(Aircraft *first_aircraft, Aircraft *second_aircraft, double time_to_conflict)
        : first_aircraft(first_aircraft), second_aircraft(second_aircraft), time_to_conflict(time_to_conflict) {}
