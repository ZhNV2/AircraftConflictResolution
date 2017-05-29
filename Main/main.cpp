include <iostream>
#include "../DomainData/Aircraft/aircraft.h"
#include "Runner/runner.h"


int main() {
	//freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	std::cout << "Main has started" << std::endl;
	Runner runner = Runner(5);
	runner.Run();
	return 0;
}