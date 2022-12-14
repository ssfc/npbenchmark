# include "PCenter.h"
# include "pcp_vector.h"

# include <algorithm>
# include <ctime>
# include <iostream>
# include <random>


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

        // 1: greedy method;
        // PCP_Greedy test_graph(input.nodeNum, input.centerNum, input.coverages, input.nodesWithDrops, seed);
        // test_graph.greedy_find(input.nodeNum, input.centerNum, input.coverages, input.nodesWithDrops, seed);

        // 2: local search method;
        PCP_Vector test_graph(input.nodeNum, input.centerNum, input.coverages, input.nodesWithDrops, seed);
        double start_time = clock();
        unsigned int test_iterations = test_graph.local_search();
        double end_time = clock();
        double elapsed_time = (end_time - start_time) / CLOCKS_PER_SEC;
        cerr << "success, iterations: " << test_iterations << " elapsed_time(s): " << elapsed_time
             << " frequency:" << test_iterations / elapsed_time << endl;

        for (int i = 0;i < input.centerNum;i++)
        {
            output[i] = test_graph.get_solution(i);
        }

		// TODO: the following code in this function is for illustration only and can be deleted.
		// print some information for debugging.
        cerr << "iterations: " << test_iterations << endl;
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
