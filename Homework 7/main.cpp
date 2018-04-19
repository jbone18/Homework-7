#include "Simulator.h"

Simulator airport;

int main() 
{

	airport.enter_data();
	airport.run_simulation();
	airport.show_stats();

	int ans;
	std::cin >> ans;
	return 0;
} 