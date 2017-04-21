//
// Created by Нико on 21.04.2017.
//

#include <ctime>
#include <afxres.h>
#include "runner.h"

Runner::Runner(int aircraft_number) {
    domain_objects_collection = new DomainObjectsCollection(aircraft_number);
}

void Runner::Run() {

    double last_time = 0;
    while (true) {
        double cur_time = clock() / CLOCKS_PER_SEC;
        domain_objects_collection->Update(cur_time - last_time);
        last_time = cur_time;
        if (domain_objects_collection->IsFlyingYet()) {
            Sleep(SLEEP_TIME);
        } else {
            break;
        }
    }

}
