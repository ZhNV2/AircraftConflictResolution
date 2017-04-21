//
// Created by Нико on 21.04.2017.
//

#ifndef ACR_DOMAIN_OBJECTS_COLLECTION_H
#define ACR_DOMAIN_OBJECTS_COLLECTION_H

#include "../DomainObject.h"
#include "../Aircraft/aircraft.h"
#include "../ConflictManager/conflict_manager.h"

class DomainObjectsCollection : public DomainObject {
private:
    const double AIRCRAFT_VELOCITY = 3600 ; // km/h
    std::vector<Aircraft *> aircrafts;
    ConflictManager *conflictManager;
    ~DomainObjectsCollection() override;
public:
    DomainObjectsCollection(int);
    void Update(double) override;
    bool IsFlyingYet();
};
#endif //ACR_DOMAIN_OBJECTS_COLLECTION_H
