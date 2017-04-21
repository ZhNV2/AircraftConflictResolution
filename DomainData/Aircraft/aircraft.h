//
// Created by Нико on 21.04.2017.
//

#ifndef ACR_AIRCRAFT_H
#define ACR_AIRCRAFT_H


#include "../DomainObject.h"
#include "../../Geometry/Track/track.h"

class Aircraft : public DomainObject {
public:
    Aircraft(Track *, double);
    void Update(double) override;
    Point GetPosition();
    bool IsFlying();
    Track GetPrediction(double);
    double GetVelocity();
private:
    Track *track;
    size_t pos_in_track;
    double time_from_point;
    double velocity; // km/s
    bool is_flying;
    Point position;
    const double EPS = 1e-9;
    ~Aircraft();
};



#endif //ACR_AIRCRAFT_H
