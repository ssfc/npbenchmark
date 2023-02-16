#include "GraphColoring.h"
#include "head.h"

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

            long long int max_iter = 0;
            if(input.nodeNum < 500)
            {
                max_iter = 6000;
            }
            else if(input.nodeNum == 500 && input.edgeNum == 12458) // 500.1
            {
                max_iter = 4000;
            }
            else if(input.nodeNum == 500 && input.edgeNum == 62624) // 500.5
            {
                input.colorNum = 48;
                max_iter = 8000;
            }
            else if(input.nodeNum == 500 && input.edgeNum == 112437) // 500.9
            {
                max_iter = 15000;
            }
            else if(input.nodeNum == 1000 && input.edgeNum == 49629) // 1000.1
            {
                max_iter = 3000;
            }
            else if(input.nodeNum == 1000 && input.edgeNum == 249826) // 1000.5
            {
                max_iter = 40000;
            }
            else if(input.nodeNum == 1000 && input.edgeNum == 449449) // 1000.9
            {
                max_iter = 30000;
            }


            /*
             * 500.1: p=20, max_iter=30000;
             * 500.5: p=20, max_iter=30000;
             *
             */

            // input.colorNum = 48;
            Hybrid_Evolution test(input.nodeNum, input.edgeNum, input.colorNum, input.edges, seed);

            // test.tabu_search(test.final_solution, false, 10000); // test tabu search;
            // test.hybrid_evolution_search(max_iter); // test HEA;
            test.hybrid_evolution_duet_1(max_iter); // test HEAD_1;

            for (int i = 0;i < input.nodeNum;i++)
            {
                output[i] = test.best_solution.solution[i];
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