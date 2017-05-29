//
// Created by Нико on 21.04.2017.
//

#include <iostream>
#include "domain_objects_collection.h"
#include "../ConflictSolver/conflict_solver.h"
#include "../Drawer/drawer.h"

DomainObjectsCollection::DomainObjectsCollection(int aircraft_number) {
    TrackFactory trackFactory = TrackFactory();
    for (int i = 0; i < aircraft_number; i++) {
        Track_ptr track = trackFactory.GenerateTrack();
		//Track ne = track->GetSlice(1.5, 16.7, 1.0);
        objects_.push_back(std::make_shared<Aircraft>(Aircraft(track, AIRCRAFT_VELOCITY, i)));
    }
    auto air_crafts = aircrafts();
	ConflictManager_ptr conflict_manager = std::make_shared<ConflictManager>(ConflictManager(air_crafts));
    objects_.push_back(conflict_manager);
	objects_.push_back(std::make_shared<ConflictSolver>(ConflictSolver(conflict_manager)));
	objects_.push_back(std::make_shared<Drawer>(Drawer(air_crafts)));
}

void DomainObjectsCollection::Update(double time) {
    std::cout << "new update time=" << time << std::endl;
    for (auto obj : objects_) {
        obj->Update(time);
    }
}

bool DomainObjectsCollection::IsFlyingYet() {
    bool is_flying_yet = false;
    auto air_crafts = aircrafts();
    for (auto aircraft : air_crafts) {
        is_flying_yet |= aircraft->IsFlying();
    }
    return is_flying_yet;
}

std::vector<Aircraft_ptr> DomainObjectsCollection::aircrafts() {
    std::vector<Aircraft_ptr> aircrafts;
    for (auto obj : objects_) {
        Aircraft_ptr aircraft_ptr = std::dynamic_pointer_cast<Aircraft>(obj);
        if (aircraft_ptr) {
            aircrafts.push_back(aircraft_ptr);
        }
    }
    return aircrafts;
}
