#pragma once

#include "../../DomainData/Aircraft/aircraft.h"
#include "../../DomainData/Collection/domain_objects_collection.h"

class Runner {
private:
	DomainObjectsCollection_ptr domain_objects_collection_;
	const unsigned long SLEEP_TIME = 1000; // ms
public:
	Runner(int);
	void Run();

};
