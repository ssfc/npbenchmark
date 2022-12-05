# include "PCenter.h"

# include <random>
# include <iostream>
# include <set>

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
	void solve(Centers& output, PCenter& input, std::function<bool()> isTimeout, int seed) {
		initRand(seed);

		coverAllNodesUnderFixedRadius(output, input, isTimeout, seed);
		for (auto r = input.nodesWithDrops.begin(); !isTimeout() && (r != input.nodesWithDrops.end()); ++r) {
			reduceRadius(input, *r);
			coverAllNodesUnderFixedRadius(output, input, isTimeout, seed);
		}
	}

	void coverAllNodesUnderFixedRadius(Centers& output, PCenter& input, std::function<bool()> isTimeout, int seed) {
		// TODO: implement your own solver which fills the `output` to replace the following trivial solver.
		// sample solver: pick center randomly (the solution can be infeasible).

		//                      +----[ exit before timeout ]
		//                      |
		for (NodeId n = 0; !isTimeout() && (n < input.centerNum); ++n) { output[n] = rand(input.nodeNum); }
		//                                                                             |
		//        [ use the random number generator initialized by the given seed ]----+

        if(input.nodesWithDrops.size()==0)
        {
            set<int> covered; // points already covered by set;
            set<int> uncovered; // points has not been covered by set;

            for(int i=0;i<input.nodeNum;i++)
            {
                uncovered.insert(i);
            }

            cerr << "The elements in initial uncovered are: ";
            for (auto it = uncovered.begin(); it != uncovered.end(); it++)
                cerr << *it << " ";
            cerr << endl;

            for(int i=0;i<1;i++) // do one iteration;
            {
                for(int j=0;j<1;j++) // consider only one set;
                {
                    cerr << "The elements in first set are: ";
                    for (auto it = input.coverages[0].begin(); it != input.coverages[0].end(); it++)
                        cerr << *it << " ";
                    cerr << endl;

                }
            }

        }


		// TODO: the following code in this function is for illustration only and can be deleted.
		// print some information for debugging.
		cerr << input.nodeNum << '\t' << input.centerNum << endl;
		for (NodeId n = 0; !isTimeout() && (n < input.centerNum); ++n) { cerr << n << '\t' << output[n] << endl; }
	}

	void reduceRadius(PCenter& input, Nodes nodesWithDrop) {
		for (auto n = nodesWithDrop.begin(); n != nodesWithDrop.end(); ++n) {
			input.coverages[*n].pop_back();
		}
	}
};

// solver.
void solvePCenter(Centers& output, PCenter& input, std::function<bool()> isTimeout, int seed) {
	Solver().solve(output, input, isTimeout, seed);
}

}
