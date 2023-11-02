#include <iostream>
#include <string>
#include <chrono>

#include "PCenter.hpp"


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
    cerr << "maxEdgeLenRank: " << maxEdgeLenRank << endl;
    cerr << "minEdgeLenRank: " << minEdgeLenRank << endl;
    pc.current_radius = maxEdgeLenRank;
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

// (1) debug on laptop by clion:
// .\SDK_PCP.exe 999999 1 <C:\wamp64\www\npbenchmark\npbenchmark-main\SDK.PCP\data\pmed01.n100p005.txt >sln.pmed01.n100p005.txt
// (2) debug on laptop by g++ in command line:
// cd C:\wamp64\www\npbenchmark\npbenchmark-main\SDK.PCP
// g++ -static-libgcc -static-libstdc++ -I C:\boost_1_81_0 Main.cpp PCenter.cpp pcp_vector.cpp -O3 && .\a.exe 999999 1 <C:\wamp64\www\npbenchmark\npbenchmark-main\SDK.PCP\data\pmed01.n100p005.txt >sln.txt
// (3) debug on ubuntu gdb:
// g++ Main.cpp PCenter.cpp pcp_vector.cpp -g && gdb a.out
// r 999999 1 <./data/pmed01.n100p005.txt >sln.txt
// r 999999 1 <./data/pcb3038p200r141.txt  >sln.txt