//
// Created by Нико on 21.04.2017.
//

#include <iostream>
#include "domain_objects_collection.h"

DomainObjectsCollection::DomainObjectsCollection(int aircraft_number) {
    TrackFactory trackFactory = TrackFactory();
    for (int i = 0; i < aircraft_number; i++) {
        Track *track = trackFactory.GenerateTrack();
        aircrafts.push_back(new Aircraft(track, AIRCRAFT_VELOCITY));
    }
    conflictManager = new ConflictManager(aircrafts);
}

void DomainObjectsCollection::Update(double time) {
    std::cout << "new update time=" << time << std::endl;
    for (auto aircraft : aircrafts) {
        aircraft->Update(time);
    }
    conflictManager->Update(time);
}

bool DomainObjectsCollection::IsFlyingYet() {
    bool is_flying_yet = false;
    for (auto aircraft : aircrafts) {
        is_flying_yet |= aircraft->IsFlying();
    }
    return is_flying_yet;
}

DomainObjectsCollection::~DomainObjectsCollection() {

}
