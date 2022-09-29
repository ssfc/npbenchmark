#include <iostream>
#include <string>
#include <chrono>

#include "VRPTW2d.h"


using namespace std;
using namespace szx;


void loadInput(istream& is, VRPTW2d& vrp) {
	is >> vrp.nodeNum >> vrp.maxVehicleNum >> vrp.vehicleCapacity;
	vrp.nodes.resize(vrp.nodeNum);
	for (auto n = vrp.nodes.begin(); n != vrp.nodes.end(); ++n) {
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
