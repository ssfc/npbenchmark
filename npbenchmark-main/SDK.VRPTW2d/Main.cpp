#include <iostream>
#include <string>
#include <chrono>

#include "VRPTW2d.h"


using namespace std;
using namespace szx;


void loadInput(istream& is, VRPTW2d& vrp) {
	is >> vrp.nodeNum >> vrp.maxVehicleNum >> vrp.vehicleCapacity;
	vrp.nodes.resize(vrp.nodeNum);
	for (auto & node : vrp.nodes) {
		is >> node.coords[0] >> node.coords[1] >> node.demand >> node.min_stay_time >> node.window_begin >> node.window_end;
	}
}

void saveOutput(ostream& os, Routes& routes) {
	for (auto & route : routes) {
		if (route.route.empty()) { continue; }
		for (int & node : route.route) { os << node << ' '; }
		os << endl;
	}
}

void test(istream& inputStream, ostream& outputStream, long long secTimeout, int randSeed) {
	cerr << "load input." << endl;
	VRPTW2d vrp;
	loadInput(inputStream, vrp);

	cerr << "solve." << endl;
	chrono::steady_clock::time_point endTime = chrono::steady_clock::now() + chrono::seconds(secTimeout);
	Routes routes(vrp.maxVehicleNum);
	solveVRPTW2d(routes, vrp, [&]() -> bool { return endTime < chrono::steady_clock::now(); }, randSeed);

	cerr << "save output." << endl;
	saveOutput(outputStream, routes);
}
void test(istream& inputStream, ostream& outputStream, long long secTimeout) {
	return test(inputStream, outputStream, secTimeout, static_cast<int>(time(nullptr) + clock()));
}


int main(int argc, char* argv[]) {
	cerr << "load environment." << endl;
	if (argc > 2) {
        char* endptr;
        long long secTimeout = strtoll(argv[1], &endptr, 10);
        if (*endptr != '\0') {
            std::cerr << "Error: Invalid argument '" << argv[1] << "'\n";
            return 1;
        }

		int randSeed = atoi(argv[2]);
		test(cin, cout, secTimeout, randSeed);
	} else {
		//ifstream ifs("path/to/instance.txt");
		//ofstream ofs("path/to/solution.txt");
		//test(ifs, ofs, 10); // for self-test.
	}
	return 0;
}

/*
 * laptop path:
 * C:\wamp64\www\npbenchmark\npbenchmark-main\SDK.VRPTW2d
 * home server path:
 * /home/ssfc/SDK.VRPTW2d
 * Compiling and running command:
 * home server:
 * g++ Main.cpp VRPTW2d.cpp AntColony.cpp -O3 && ./a.out 999999 1 <./data/solomon.c101.n101v25c200.txt  >sln.txt
 */