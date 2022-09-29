#include <iostream>
#include <sstream>
#include <string>
#include <chrono>

#include "DFeedbackVertexSet.h"


using namespace std;
using namespace szx;


void loadInput(istream& is, DFeedbackVertexSet& dfvs) {
	is >> dfvs.nodeNum >> dfvs.arcNum;
	string s;
	getline(is, s);
	dfvs.adjList.resize(dfvs.nodeNum);
	for (NodeId src = 0; src < dfvs.nodeNum; ++src) {
		getline(is, s);
		istringstream iss(s);
		for (NodeId dst; iss >> dst; dfvs.adjList[src].push_back(dst)) {}
	}
}

void saveOutput(ostream& os, Nodes& removedNodes) {
	for (auto n = removedNodes.begin(); n != removedNodes.end(); ++n) { os << *n << endl; }
}

void test(istream& inputStream, ostream& outputStream, long long secTimeout, int randSeed) {
	cerr << "load input." << endl;
	DFeedbackVertexSet dfvs;
	loadInput(inputStream, dfvs);

	cerr << "solve." << endl;
	chrono::steady_clock::time_point endTime = chrono::steady_clock::now() + chrono::seconds(secTimeout);
	Nodes removedNodes;
	solveDFeedbackVertexSet(removedNodes, dfvs, [&]() -> bool { return endTime < chrono::steady_clock::now(); }, randSeed);

	cerr << "save output." << endl;
	saveOutput(outputStream, removedNodes);
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
