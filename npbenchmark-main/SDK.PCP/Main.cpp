#include <iostream>
#include <string>
#include <chrono>

#include "PCenter.h"


using namespace std;
using namespace szx;


void loadInput(istream& is, PCenter& pc) {
	is >> pc.nodeNum >> pc.centerNum;
	pc.coverages.resize(pc.nodeNum);
	for (auto coverage = pc.coverages.begin(); coverage != pc.coverages.end(); ++coverage) {
		NodeId coveredNodeNum;
		is >> coveredNodeNum;
		coverage->resize(coveredNodeNum);
		for (auto node = coverage->begin(); node != coverage->end(); ++node) { is >> *node; }
	}

	EdgeId minEdgeLenRank;
	EdgeId maxEdgeLenRank;
	is >> maxEdgeLenRank >> minEdgeLenRank;
	pc.nodesWithDrops.resize(maxEdgeLenRank - minEdgeLenRank);
	for (auto r = pc.nodesWithDrops.begin(); r != pc.nodesWithDrops.end(); ++r) {
		NodeId nodeNumToDrop;
		is >> nodeNumToDrop;
		r->resize(nodeNumToDrop);
		for (auto node = r->begin(); node != r->end(); ++node) { is >> *node; }
	}
}

void saveOutput(ostream& os, Centers& centers) {
	for (auto center = centers.begin(); center != centers.end(); ++center) { os << *center << endl; }
}

void test(istream& inputStream, ostream& outputStream, long long secTimeout, int randSeed) {
	cerr << "load input." << endl;
	PCenter pc;
	loadInput(inputStream, pc);

	cerr << "solve." << endl;
	chrono::steady_clock::time_point endTime = chrono::steady_clock::now() + chrono::seconds(secTimeout);
	Centers centers(pc.centerNum);
	solvePCenter(centers, pc, [&]() -> bool { return endTime < chrono::steady_clock::now(); }, randSeed);

	cerr << "save output." << endl;
	saveOutput(outputStream, centers);
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
