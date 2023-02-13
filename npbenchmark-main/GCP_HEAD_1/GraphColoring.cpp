#include "GraphColoring.h"
#include "hea.h"

#include <fstream>
#include <iostream>
#include <random>


using namespace std;

namespace szx
{
    class Solver{
        // random number generator.
        mt19937 pseudoRandNumGen;
        void initRand(int seed) { pseudoRandNumGen = mt19937(seed); }
        int fastRand(int lb, int ub) { return (pseudoRandNumGen() % (ub - lb)) + lb; }
        int fastRand(int ub) { return pseudoRandNumGen() % ub; }
        int rand(int lb, int ub) { return uniform_int_distribution<int>(lb, ub - 1)(pseudoRandNumGen); }
        int rand(int ub) { return uniform_int_distribution<int>(0, ub - 1)(pseudoRandNumGen); }

    public:
        void solve(NodeColors& output, GraphColoring& input, std::function<bool()> isTimeout, int seed) {
            /*
            if(input.nodeNum==500 && input.colorNum==12)
            {
                seed = 6;
            }
             */

            // TODO: implement your own solver which fills the `output` to replace the following trivial solver.
            // sample solver: assign colors randomly (the solution can be infeasible).

            //                      +----[ exit before timeout ]
            //

            /*
            for (NodeId n = 0; !isTimeout() && (n < input.nodeNum); ++n)
            {
                output[n] = rand(input.colorNum);
            }
            */

            int num_population = 20;
            long long int max_iter = 30000;

            /*
             * 500.1: p=20, max_iter=30000;
             * 500.5: p=20, max_iter=30000;
             *
             */

            // input.colorNum = 48;
            Hybrid_Evolution test(input.nodeNum, input.edgeNum, input.colorNum, input.edges, num_population, seed);

            // test.tabu_search(test.final_solution, false, 10000);
            test.hybrid_evolution_search(max_iter);

            for (int i = 0;i < input.nodeNum;i++)
            {
                output[i] = test.final_solution[i];
            }

            // debug function:
            /*
            cerr << "output: " << endl;
            for(int i=0;i<input.nodeNum;i++)
            {
                {
                    cerr << output[i] << " ";
                }
            }*/
            cerr << endl;
            //*/// to reduce
        }
    };

    // solver.
    void solveGraphColoring(NodeColors& output, GraphColoring& input, std::function<bool()> isTimeout, int seed) {
        Solver().solve(output, input, isTimeout, seed);
    }
}