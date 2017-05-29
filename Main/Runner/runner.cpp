//
// Created by Нико on 21.04.2017.
//

#include <ctime>
#include <afxres.h>
#include <Windows.h>
#include "runner.h"

Runner::Runner(int aircraft_number) {
	domain_objects_collection_ = std::make_shared<DomainObjectsCollection>(DomainObjectsCollection(aircraft_number));
}

void Runner::Run() {

	double last_time = 0;
	while (true) {
		double cur_time = clock() / CLOCKS_PER_SEC;
		     domain_objects_collection_->Update(cur_time - last_time);
		last_time = cur_time;
		if (domain_objects_collection_->IsFlyingYet()) {
			Sleep(SLEEP_TIME);
		}
		else {
			break;
		}
	}
}
