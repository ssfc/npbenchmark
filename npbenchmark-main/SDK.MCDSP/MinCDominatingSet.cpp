#include "MinCDominatingSet.h"

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
	void solve(Nodes& output, MinCDominatingSet& input, std::function<bool()> isTimeout, int seed) {
		initRand(seed);

		// TODO: implement your own solver which fills the `output` to replace the following trivial solver.
		// sample solver: pick node randomly (the solution can be infeasible).

		//                      +----[ exit before timeout ]
		//                      |
		for (NodeId n = 0; !isTimeout() && (n < 100); ++n) { output[n] = rand(input.nodeNum); }
		//                                                                           |
		//      [ use the random number generator initialized by the given seed ]----+

		// TODO: the following code in this function is for illustration only and can be deleted.
		// print some information for debugging.
		cerr << input.nodeNum << '\t' << input.edgeNum << endl;
		for (auto n = output.begin(); !isTimeout() && (n != output.end()); ++n) { cerr << *n << endl; }
	}
};

// solver.
void solveMinCDominatingSet(Nodes& output, MinCDominatingSet& input, std::function<bool()> isTimeout, int seed) {
	Solver().solve(output, input, isTimeout, seed);
}

}
