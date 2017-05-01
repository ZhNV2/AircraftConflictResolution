	//
// Created by Нико on 21.04.2017.
//

#ifndef ACR_AIRCRAFT_H
#define ACR_AIRCRAFT_H


#include "../domain_object.h"
#include "../../Geometry/Track/track.h"

class Aircraft : public DomainObject {
public:
    Aircraft(Track_ptr, double, int);
    void Update(double) override;
    Point GetPosition() const;
    bool IsFlying() const;
    Track GetPrediction(double);
    double GetVelocity() const;
	Track GetLastPredicion() const;
	int GetID() const;
	void InsertTrack(Point p, const Track&);
	~Aircraft();
private:
	int ID;
	double flown;
    Track_ptr track;
	Track last_prediction;
    size_t pos_in_track;
    double time_from_point;
    double velocity; // km/s
    bool is_flying;
    Point position;
    const double EPS = 1e-9;
};

typedef std::shared_ptr<Aircraft> Aircraft_ptr;



#endif //ACR_AIRCRAFT_H
