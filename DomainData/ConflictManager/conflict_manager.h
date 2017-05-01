//
// Created by Нико on 21.04.2017.
//

#ifndef ACR_CONFLICT_MANAGER_H
#define ACR_CONFLICT_MANAGER_H

#include <vector>
#include "../domain_object.h"
#include "../Aircraft/aircraft.h"

class ConflictHolder;

class ConflictManager : public DomainObject {
private:
    std::vector<Aircraft_ptr> aircrafts;
    std::vector<ConflictHolder> conflicts;
public:
	static double CheckIntersection(const Track &, const Track &, double velocity1, double velocity2);
    ConflictManager(std::vector<Aircraft_ptr>);
	static double GetPredictionTime();
	std::vector<ConflictHolder>& GetConflicts();
	std::vector<Aircraft_ptr>& GetAircrafts();
    void Update(double) override;
	~ConflictManager() override;
};

typedef std::shared_ptr<ConflictManager> ConflictManager_ptr;

class ConflictHolder {
public:
    ConflictHolder(Aircraft_ptr first_aircraft, Aircraft_ptr second_aircraft, double time_to_conflict);

public:
    Aircraft_ptr first_aircraft;
    Aircraft_ptr second_aircraft;
    double time_to_conflict;
};

typedef std::shared_ptr<ConflictManager> ConflictManager_ptr;

#endif //ACR_CONFLICT_MANAGER_H
