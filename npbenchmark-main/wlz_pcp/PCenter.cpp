# include "PCenter.h"
# include "vwts.h"

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

            VWTS test(input.nodeNum, input.centerNum,
                      input.coverages, input.nodesWithDrops, seed);
            test.vertex_weight_tabu_search(seed);
            test.get_solution(output);

            cerr << "Evaluate output: ";
            for(int i=0;i<input.centerNum;i++)
            {
                cerr << output[i] << " ";
            }
            cerr << endl;


            // printf("iter:%d time:%lf  uncovered_num:%d", test.tempiter,((double)test.end_ms - (double)test.start_ms) / 1000.0, test.tempnum);


            //*/
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