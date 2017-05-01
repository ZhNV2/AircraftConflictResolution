#pragma once

#ifndef ACR_TIME_SERVER_H
#define ACR_TIME_SERVER_H

#include "../../DomainData/Aircraft/aircraft.h"
#include "../../DomainData/Collection/domain_objects_collection.h"

class Runner {
private:
	DomainObjectsCollection_ptr domain_objects_collection;
	const unsigned long SLEEP_TIME = 1000; // ms
public:
	Runner(int);
	void Run();

};

#endif //ACR_TIME_SERVER_H
