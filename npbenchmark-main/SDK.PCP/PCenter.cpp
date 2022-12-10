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

        initRand(seed); // initialize random generator;

        if(input.nodesWithDrops.size()==0)
        {
            vector<int> universe; // all points;
            vector<int> covered; // points already covered by set;
            vector<int> uncovered; // points has not been covered by set;
            vector<int> selected; // centers selected;
            int equal_delta[2000]; //非禁忌相同delta值
            int equal_count = 0;

            for(int i=0;i<input.nodeNum;i++)
            {
                universe.push_back(i);
            }

            for(int i=0;i<input.nodeNum;i++)
            {
                uncovered.push_back(i);
            }

            /* // test whether union two sets is correct or not;
            vector<int> test_merge;
            set_union(input.coverages[59].begin(),input.coverages[59].end(),
                             input.coverages[12].begin(),input.coverages[12].end(),
                             back_inserter(test_merge));
            cerr << "Test merge size (" <<test_merge.size()<<"): ";
            for (int & it : test_merge)
                cerr << it << " ";
            cerr << endl;
            */

            for(int i=0; selected.size()<input.centerNum && covered.size()!=input.nodeNum; i++) // do one iteration;
            {
                cerr<<"iteration: "<<i<<endl;
                int max_overlap_size = 0;
                int max_overlap_index = 0;
                for(int j=0;j<input.nodeNum;j++) // consider only one set;
                {
                    vector<int> this_intersection;
                    set_intersection(uncovered.begin(),uncovered.end(),
                                     input.coverages[j].begin(),input.coverages[j].end(),
                                     back_inserter(this_intersection));

                    if(this_intersection.size() > max_overlap_size)
                    {
                        max_overlap_size = this_intersection.size();
                        max_overlap_index = j;

                        equal_count = 0;
                        equal_delta[equal_count] = j; // j is index of center;
                        equal_count++;
                    }
                    else if(this_intersection.size() == max_overlap_size)
                    {
                        equal_delta[equal_count] = j; // j is index of center;
                        equal_count++;
                    }
                }

                cerr << "equal count: " << equal_count << endl;

                cerr << "max_overlap_index: " << max_overlap_index <<endl;
                int rand_select = pseudoRandNumGen() % equal_count; // 相等tabu_delta随机选择
                cerr << "random select: " << rand_select <<endl;
                cerr << "random select index: " << equal_delta[rand_select] <<endl;

                selected.push_back(max_overlap_index);
                vector<int> union_result;
                set_union(covered.begin(),covered.end(),
                          input.coverages[max_overlap_index].begin(),input.coverages[max_overlap_index].end(),
                          back_inserter(union_result));

                cerr << "The results of union "<<max_overlap_index<<" are: ";
                for (int & it : union_result)
                    cerr << it << " ";
                cerr << endl;

                covered.assign(union_result.begin(), union_result.end());

                cerr << "Cover after union size (" << covered.size() << "): ";
                for (int & it : union_result)
                    cerr << it << " ";
                cerr << endl;

                vector<int> difference_result;
                set_difference(universe.begin(),universe.end(),
                          covered.begin(),covered.end(),
                          back_inserter(difference_result));

                cerr << "Uncover after union are: ";
                for (int & it : difference_result)
                    cerr << it << " ";
                cerr << endl;

                uncovered.assign(difference_result.begin(), difference_result.end());

            }

            cerr << "Point selected are: ";
            for (int & it : selected)
                cerr << it << " ";
            cerr << endl;

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
