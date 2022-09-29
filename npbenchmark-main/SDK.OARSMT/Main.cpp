#include <iostream>
#include <string>
#include <chrono>

#include "OARSteinerMinTree.h"


using namespace std;
using namespace szx;


void loadInput(istream& is, OARSteinerMinTree& oarsmt) {
	is >> oarsmt.nodeNum >> oarsmt.obstacleNum;
	oarsmt.nodes.resize(oarsmt.nodeNum);
	for (auto node = oarsmt.nodes.begin(); node != oarsmt.nodes.end(); ++node) { is >> (*node)[0] >> (*node)[1]; }
	oarsmt.obstacles.resize(oarsmt.obstacleNum);
	for (auto obstacle = oarsmt.obstacles.begin(); obstacle != oarsmt.obstacles.end(); ++obstacle) {
		is >> (*obstacle)[0] >> (*obstacle)[1] >> (*obstacle)[2] >> (*obstacle)[3];
	}
}

void saveOutput(ostream& os, Paths& paths) {
	for (auto p = paths.begin(); p != paths.end(); ++p) {
		os << (*p)[0][0] << " " << (*p)[0][1];
		for (auto n0 = p->begin(), n1 = n0 + 1; n1 != p->end(); ++n0, ++n1) {
			Coord dx = (*n1)[0] - (*n0)[0];
			if (dx != 0) {
				os << " x " << dx;
			} else {
				os << " y " << ((*n1)[1] - (*n0)[1]);
			}
		}
		os << endl;
	}
}

void test(istream& inputStream, ostream& outputStream, long long secTimeout, int randSeed) {
	cerr << "load input." << endl;
	OARSteinerMinTree oarsmt;
	loadInput(inputStream, oarsmt);

	cerr << "solve." << endl;
	chrono::steady_clock::time_point endTime = chrono::steady_clock::now() + chrono::seconds(secTimeout);
	Paths paths;
	solveOARSteinerMinTree(paths, oarsmt, [&]() -> bool { return endTime < chrono::steady_clock::now(); }, randSeed);

	cerr << "save output." << endl;
	saveOutput(outputStream, paths);
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
