#include <iostream>
#include <string>
#include <chrono>

#include "FJSP.h"


using namespace std;
using namespace szx;


void loadInput(istream& is, FJSP& fjsp) {
	is >> fjsp.jobNum >> fjsp.workerNum >> fjsp.maxCandidateNum;
	fjsp.jobs.resize(fjsp.jobNum);
	for (auto job = fjsp.jobs.begin(); job != fjsp.jobs.end(); ++job) {
		OperationId operationNum;
		is >> operationNum;
		job->resize(operationNum);
		for (auto op = job->begin(); op != job->end(); ++op) {
			WorkerId candidateNum;
			is >> candidateNum;
			op->resize(candidateNum);
			for (auto candidate = op->begin(); candidate != op->end(); ++candidate) {
				is >> candidate->worker >> candidate->duration;
			}
		}
	}
}

void saveOutput(ostream& os, Schedule& schedule) {
	for (auto worker = schedule.begin(); worker != schedule.end(); ++worker) {
		os << worker->size();
		for (auto task = worker->begin(); task != worker->end(); ++task) {
			os << ' ' << task->job << ' ' << task->operation;
		}
		os << endl;
	}
}

void test(istream& inputStream, ostream& outputStream, long long secTimeout, int randSeed) {
	cerr << "load input." << endl;
	FJSP fjsp;
	loadInput(inputStream, fjsp);

	cerr << "solve." << endl;
	chrono::steady_clock::time_point endTime = chrono::steady_clock::now() + chrono::seconds(secTimeout);
	Schedule schedule(fjsp.workerNum);
	solveFJSP(schedule, fjsp, [&]() -> bool { return endTime < chrono::steady_clock::now(); }, randSeed);

	cerr << "save output." << endl;
	saveOutput(outputStream, schedule);
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
