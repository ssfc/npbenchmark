#include <iostream>
#include <string>
#include <chrono>

#include "GraphColoring.h"


using namespace std;
using namespace szx;


void loadInput(istream& is, GraphColoring& gc) {
	is >> gc.nodeNum >> gc.edgeNum >> gc.colorNum;
	gc.edges.resize(gc.edgeNum);
	for (auto edge = gc.edges.begin(); edge != gc.edges.end(); ++edge) { is >> (*edge)[0] >> (*edge)[1]; }
}

void saveOutput(ostream& os, NodeColors& nodeColors) {
	for (auto color = nodeColors.begin(); color != nodeColors.end(); ++color) { os << *color << endl; }
}

void test(istream& inputStream, ostream& outputStream, long long secTimeout, int randSeed) {
	cerr << "load input." << endl;
	GraphColoring gc;
	loadInput(inputStream, gc);

	cerr << "solve." << endl;
	chrono::steady_clock::time_point endTime = chrono::steady_clock::now() + chrono::seconds(secTimeout);
	NodeColors nodeColors(gc.nodeNum);
	solveGraphColoring(nodeColors, gc, [&]() -> bool { return endTime < chrono::steady_clock::now(); }, randSeed);

	cerr << "save output." << endl;
	saveOutput(outputStream, nodeColors);
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
