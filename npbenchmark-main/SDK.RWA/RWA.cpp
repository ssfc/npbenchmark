#include "RWA.h"

#include <random>
#include <iostream>


using namespace std;


namespace szx {

class Solver {
	// random number generator.
	mt19937 pseudoRandNumGen;
	void initRand(int seed) { pseudoRandNumGen = mt19937(seed); }
	int fastRand(int lb, int ub) { return (pseudoRandNumGen() % (ub - lb)) + lb; }
	int fastRand(int ub) { return pseudoRandNumGen() % ub; }
	int rand(int lb, int ub) { return uniform_int_distribution<int>(lb, ub - 1)(pseudoRandNumGen); }
	int rand(int ub) { return uniform_int_distribution<int>(0, ub - 1)(pseudoRandNumGen); }

public:
	void solve(Routes& output, RWA& input, std::function<bool()> isTimeout, int seed) {
		initRand(seed);

		// TODO: implement your own solver which fills the `output` to replace the following trivial solver.
		// sample solver: assign wavelengths and find routes randomly (the solution can be infeasible).

		//                        +----[ exit before timeout ]
		//                        |
		for (TrafficId t = 0; !isTimeout() && (t < input.trafficNum); ++t) { output[t].wavelen = rand(input.trafficNum); }
		//                                                                                        |
		//                   [ use the random number generator initialized by the given seed ]----+

		for (TrafficId t = 0; !isTimeout() && (t < input.trafficNum); ++t) {
			output[t].nodes.resize(rand(input.nodeNum));
			for (auto n = output[t].nodes.begin(); n != output[t].nodes.end(); ++n) {
				*n = rand(input.trafficNum);
			}
		}

		// TODO: the following code in this function is for illustration only and can be deleted.
		// print some information for debugging.
		cerr << input.nodeNum << '\t' << input.arcNum << '\t' << input.trafficNum << endl;
		cerr << "traffic\twavelen" << endl;
		for (TrafficId t = 0; !isTimeout() && (t < input.trafficNum); ++t) { cerr << t << '\t' << output[t].wavelen << endl; }
	}
};

// solver.
void solveRWA(Routes& output, RWA& input, std::function<bool()> isTimeout, int seed) {
	Solver().solve(output, input, isTimeout, seed);
}

}
