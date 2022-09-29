#include <iostream>
#include <string>
#include <chrono>

#include "RWA.h"


using namespace std;
using namespace szx;


void loadInput(istream& is, RWA& rwa) {
	is >> rwa.nodeNum >> rwa.arcNum >> rwa.trafficNum;
	rwa.arcs.resize(rwa.arcNum);
	for (auto arc = rwa.arcs.begin(); arc != rwa.arcs.end(); ++arc) { is >> (*arc)[0] >> (*arc)[1]; }
	rwa.traffics.resize(rwa.trafficNum);
	for (auto traffic = rwa.traffics.begin(); traffic != rwa.traffics.end(); ++traffic) { is >> (*traffic)[0] >> (*traffic)[1]; }
}

void saveOutput(ostream& os, Routes& routes) {
	for (auto route = routes.begin(); route != routes.end(); ++route) {
		os << route->wavelen << ' ' << route->nodes.size();
		for (auto node = route->nodes.begin(); node != route->nodes.end(); ++node) { os << ' ' << *node; }
		os << endl;
	}
}

void test(istream& inputStream, ostream& outputStream, long long secTimeout, int randSeed) {
	cerr << "load input." << endl;
	RWA rwa;
	loadInput(inputStream, rwa);

	cerr << "solve." << endl;
	chrono::steady_clock::time_point endTime = chrono::steady_clock::now() + chrono::seconds(secTimeout);
	Routes routes(rwa.trafficNum);
	solveRWA(routes, rwa, [&]() -> bool { return endTime < chrono::steady_clock::now(); }, randSeed);

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
