#pragma once

#include "../domain_object.h"
#include "../../Geometry/Track/track.h"

class Aircraft : public DomainObject {
public:
    Aircraft(Track_ptr, double, int);
	Track GetTrack();
    void Update(double) override;
    Point GetPosition() const;
    bool IsFlying() const;
    Track GetPrediction(double);
    double GetVelocity() const;
	Track GetLastPredicion() const;
	int GetID() const;
	void InsertTrack(Point p, const Track&);
private:
	int ID_;
	double flown_;
    Track_ptr track_;
	Track last_prediction_;
    size_t pos_in_track_;
    double time_from_point_;
    double velocity_; // km/s
    bool is_flying_;
    Point position_;
    const double EPS = 1e-9;
};

typedef std::shared_ptr<Aircraft> Aircraft_ptr;
