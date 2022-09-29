#include <iostream>
#include <string>
#include <chrono>

#include "RectPacking.h"


using namespace std;
using namespace szx;


void loadInput(istream& is, RectPacking& rp) {
	is >> rp.rectNum;
	rp.rects.resize(rp.rectNum);
	for (auto rect = rp.rects.begin(); rect != rp.rects.end(); ++rect) { is >> (*rect)[0] >> (*rect)[1]; }
}

void saveOutput(ostream& os, Layout& layout) {
	for (auto placement = layout.begin(); placement != layout.end(); ++placement) {
		os << placement->pos[0] << ' ' << placement->pos[1] << ' ' << (placement->rotated ? "90" : "0") << endl;
	}
}

void test(istream& inputStream, ostream& outputStream, long long secTimeout, int randSeed) {
	cerr << "load input." << endl;
	RectPacking rp;
	loadInput(inputStream, rp);

	cerr << "solve." << endl;
	chrono::steady_clock::time_point endTime = chrono::steady_clock::now() + chrono::seconds(secTimeout);
	Layout layout(rp.rectNum);
	solveRectPacking(layout, rp, [&]() -> bool { return endTime < chrono::steady_clock::now(); }, randSeed);

	cerr << "save output." << endl;
	saveOutput(outputStream, layout);
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
