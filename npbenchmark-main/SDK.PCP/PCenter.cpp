# include "PCenter.h"

# include <algorithm>
# include <random>
# include <iostream>

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
            vector<int> covered; // points already covered by set;
            vector<int> uncovered; // points has not been covered by set;

            for(int i=0;i<input.nodeNum;i++)
            {
                uncovered.push_back(i);
            }

            cerr << "The elements in initial uncovered are: ";
            for (auto it = uncovered.begin(); it != uncovered.end(); it++)
                cerr << *it << " ";
            cerr << endl;

            vector<int> intersection_result;
            set_intersection(input.coverages[0].begin(),input.coverages[0].end(),
                             input.coverages[1].begin(),input.coverages[1].end(),
                             back_inserter(intersection_result));

            cerr << "The elements in intersection set are: ";
            for (auto it = intersection_result.begin(); it != intersection_result.end(); it++)
                cerr << *it << " ";
            cerr << endl;

            for(int i=0;i<1;i++) // do one iteration;
            {
                int max_overlap_size = 0;
                int max_overlap_index = 0;
                for(int j=0;j<input.nodeNum;j++) // consider only one set;
                {
                    cerr << "The elements in first set are: ";
                    for (auto it = input.coverages[0].begin(); it != input.coverages[0].end(); it++)
                        cerr << *it << " ";
                    cerr << endl;

                    vector<int> this_intersection;
                    set_intersection(uncovered.begin(),uncovered.end(),
                                     input.coverages[j].begin(),input.coverages[j].end(),
                                     back_inserter(this_intersection));

                    cerr << "The elements in j="<<j<<" are: ";
                    for (auto it = this_intersection.begin(); it != this_intersection.end(); it++)
                        cerr << *it << " ";
                    cerr << endl;

                    cerr<<"this_intersection_size: "<<this_intersection.size()<<endl;

                    if(this_intersection.size() > max_overlap_size)
                    {
                        max_overlap_size = this_intersection.size();
                        max_overlap_index = j;
                    }

                }
                cerr << "max_overlap_size: " << max_overlap_size <<endl;
                cerr << "max_overlap_index: " << max_overlap_index <<endl;
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
