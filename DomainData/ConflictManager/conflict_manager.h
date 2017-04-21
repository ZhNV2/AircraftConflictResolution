//
// Created by Нико on 21.04.2017.
//

#ifndef ACR_CONFLICT_MANAGER_H
#define ACR_CONFLICT_MANAGER_H

#include <vector>
#include "../DomainObject.h"
#include "../Aircraft/aircraft.h"

class ConflictHolder;

class ConflictManager : public DomainObject {
private:
    std::vector<Aircraft*> &aircrafts;
    std::vector<ConflictHolder> conflicts;
    const double PREDICTION_TIME = 5; // s
    const double MIN_DIST = 1;
    const double EPS = 1e-9;
    double detectConflict(Point, Point, Vector, Vector, double);
public:
    ConflictManager(std::vector<Aircraft*> &);
    void Update(double) override;
};

class ConflictHolder {
public:
    ConflictHolder(Aircraft *first_aircraft, Aircraft *second_aircraft, double time_to_conflict);

public:
    Aircraft *first_aircraft;
    Aircraft *second_aircraft;
    double time_to_conflict;
};


#endif //ACR_CONFLICT_MANAGER_H
