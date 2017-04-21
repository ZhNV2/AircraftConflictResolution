#include <iostream>
#include "DomainData/aircraft/aircraft.h"
#include "Runner/runner.h"


int main() {
    std::cout << "Main has started" << std::endl;
    Runner runner = Runner(2);
    runner.Run();
    return 0;
}