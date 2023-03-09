#include <iostream>
#include <string>
#include <chrono>
#include <fstream>

#include "../.h/PCenter.h"
#include "../.h/UCoverX.h"
#include "../.h/Solver.h"

using namespace std;

void loadInput(istream& is, PCenter& pc, UCoverX& UX) {
	is >> pc.nodeNum >> pc.centerNum;
	pc.init();
	UX.setCapacity(pc.nodeNum);
	int coveredNodeNum, coverage = 0, node;
	for (; coverage < pc.nodeNum; coverage++) {
		pc.tabulist[coverage] = 0;
		pc.w[coverage] = 1;
		pc.Delta[coverage] = 0;
		is >> coveredNodeNum;
		pc.sizes[coverage] = coveredNodeNum;
		pc.covers[coverage].init(pc.nodeNum, coveredNodeNum);
		if(coveredNodeNum == 1){
			pc.X[pc.Xindex] = coverage;
			pc.positions[coverage] = pc.Xindex++;
			pc.covers[coverage].push_back(coverage);
		}
		else{
			UX.push_back(pc, coverage);
		}
		pc.coverages[coverage] = new int[coveredNodeNum];
		for (node = 0; node < coveredNodeNum; node++) { is >> pc.coverages[coverage][node]; }
	}

	int minEdgeLenRank, maxEdgeLenRank, r = 0, nodeNumToDrop;
	is >> maxEdgeLenRank >> minEdgeLenRank;
	pc.init(maxEdgeLenRank, minEdgeLenRank);
	// cout << "dropLen = " << pc.dropLen << endl;
	for (; r < pc.dropLen; r++) {
		is >> nodeNumToDrop;
		pc.nodesWithDrops[r] = new int[nodeNumToDrop];
		pc.dropsSizes[r] = nodeNumToDrop;
		for (node = 0; node < nodeNumToDrop; node++) { is >> pc.nodesWithDrops[r][node]; }
	}
}

void saveOutput(ostream& os, int& centerNum, int*& centers) {
	for (int center = 0; center != centerNum; center++) { os << centers[center] << endl; }
}

void test(istream& inputStream, ostream& outputStream, long long secTimeout, int randSeed) {
	cerr << "load input." << endl;
	PCenter pc;
	UCoverX UX;
	loadInput(inputStream, pc, UX);

	cerr << "solve." << endl;
	chrono::steady_clock::time_point endTime = chrono::steady_clock::now() + chrono::seconds(secTimeout);
	int *centers = new int[pc.centerNum];
	Solver().solve(centers, pc, UX, [&]() -> bool { return endTime < chrono::steady_clock::now(); }, randSeed);

	cerr << "save output." << endl;
	saveOutput(outputStream, pc.centerNum, centers);
	delete[] centers;
}
void test(istream& inputStream, ostream& outputStream, long long secTimeout) {
	return test(inputStream, outputStream, secTimeout, static_cast<int>(time(nullptr) + clock()));
}


int main(int argc, char* argv[]) {
	cerr << "load environment." << endl;

	string Abpath_prefix = "E:/C/PCenter/PCP/";
	string data_prefix = Abpath_prefix + "Instance/";
	string result_prefix = Abpath_prefix + "Result/";

	long long secTimeout;
	int randSeed;

	if (argc > 2) {
		secTimeout = atoll(argv[1]);
		randSeed = atoi(argv[2]);
		test(cin, cout, secTimeout, randSeed);
	} else {

		secTimeout = 100;

		string test_file_text  = "u1817p130.txt";			//pcb3038p010r729 pcb3038p040r337 u1817p130;
		
		string test_data_txt = data_prefix + test_file_text;
		string test_result_txt = result_prefix + test_file_text;

		ifstream ifs(test_data_txt);
		ofstream ofs(test_result_txt);

   		clock_t start_time = clock();

		test(ifs, ofs, secTimeout); // for self-test.

		cout << "Total run time is: " << (double)(clock() - start_time) / CLOCKS_PER_SEC << "s" << endl;

		system("pause");
	}
	return 0;
}
