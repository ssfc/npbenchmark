#include "GraphColoring.h"
#include "tabucol.h"

#include <climits>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

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
            initRand(seed);

            // TODO: implement your own solver which fills the `output` to replace the following trivial solver.
            // sample solver: assign colors randomly (the solution can be infeasible).

            //                      +----[ exit before timeout ]
            //

            /*               |
            for (NodeId n = 0; !isTimeout() && (n < input.nodeNum); ++n)
            {
                output[n] = rand(input.colorNum);
            }
            */

            Graph test(input.nodeNum, input.colorNum);

            // create graph;
            int v1, v2;
            int tmp;

            for (int i = 0;i < input.edgeNum; i++)
            {
                v1 = input.edges[i][0];
                v2 = input.edges[i][1];

                tmp = ++test.vertex_edge[v1];
                test.adj_list[v1][tmp - 1] = v2;
                tmp = ++test.vertex_edge[v2];
                test.adj_list[v2][tmp - 1] = v1;
            }

            // cerr << "Finish creating graph." << endl;

            // test.print_graph();

            test.initialization(seed);

            double start_time = clock();

            test.tabu_search();

            double end_time = clock();
            double elapsed_time = (end_time - start_time) / CLOCKS_PER_SEC;
            cerr << "success, iterations: " << test.iter << " elapsed_time(s): " << elapsed_time
                 << " frequency:" << test.iter / elapsed_time << endl;

            for (int i = 0;i < input.nodeNum;i++)
            {
                output[i] = test.solution[i];
            }

            test.free_memory();

            // generate all results;
            for(seed=0;seed<5;seed++)
            {
                Graph test(input.nodeNum, input.colorNum);

                // create graph;
                int vertex_1, vertex_2;
                int temp;

                for (int i = 0;i < input.edgeNum; i++)
                {
                    vertex_1 = input.edges[i][0];
                    vertex_2 = input.edges[i][1];

                    temp = ++test.vertex_edge[vertex_1];
                    test.adj_list[vertex_1][temp - 1] = vertex_2;
                    temp = ++test.vertex_edge[vertex_2];
                    test.adj_list[vertex_2][temp - 1] = vertex_1;
                }

                test.initialization(seed);

                start_time = clock();

                test.tabu_search();

                end_time = clock();
                elapsed_time = (end_time - start_time) / CLOCKS_PER_SEC;
                cerr << "success, iterations: " << test.iter << " elapsed_time(s): " << elapsed_time
                     << " frequency:" << test.iter / elapsed_time << endl;

                test.free_memory();
            }
        }
    };

    // solver.
    void solveGraphColoring(NodeColors& output, GraphColoring& input, std::function<bool()> isTimeout, int seed) {
        Solver().solve(output, input, isTimeout, seed);
    }
}
