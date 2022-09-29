#include <iostream>
#include <string>
#include <chrono>

#include "DARP2d.h"


using namespace std;
using namespace szx;


void loadInput(istream& is, DARP2d& darp) {
	is >> darp.requestNum >> darp.maxVehicleNum >> darp.vehicleCapacity >> darp.maxTravelTime >> darp.maxRideTime;
	darp.nodes.resize(darp.nodeNum());
	for (auto n = darp.nodes.begin(); n != darp.nodes.end(); ++n) {
		is >> n->coords[0] >> n->coords[1] >> n->demand >> n->minStayTime >> n->windowBegin >> n->windowEnd;
	}
}

void saveOutput(ostream& os, Routes& routes) {
	for (auto route = routes.begin(); route != routes.end(); ++route) {
		if (route->nodes.empty()) { continue; }
		for (auto node = route->nodes.begin(); node != route->nodes.end(); ++node) { os << *node << ' '; }
		os << endl;
	}
}

void test(istream& inputStream, ostream& outputStream, long long secTimeout, int randSeed) {
	cerr << "load input." << endl;
	DARP2d darp;
	loadInput(inputStream, darp);

	cerr << "solve." << endl;
	chrono::steady_clock::time_point endTime = chrono::steady_clock::now() + chrono::seconds(secTimeout);
	Routes routes(darp.maxVehicleNum);
	solveDARP2d(routes, darp, [&]() -> bool { return endTime < chrono::steady_clock::now(); }, randSeed);

	cerr << "save output." << endl;
	saveOutput(outputStream, routes);
}
void test(istream& inputStream, ostream& outputStream, long long secTimeout) {
	return test(inputStream, outputStream, secTimeout, static_cast<int>(time(nullptr) + clock()));
}

int main(int argc, char* argv[]) {
	cerr << "load environment." << endl;
	if (argc > 2) {
		long long secTimeout = atoll(argv[1]);
		int randSeed = atoi(argv[2]);
		test(cin, cout, secTimeout, randSeed);
	} else {
		//ifstream ifs("path/to/instance.txt");
		//ofstream ofs("path/to/solution.txt");
		//test(ifs, ofs, 10); // for self-test.
	}
	return 0;
}
