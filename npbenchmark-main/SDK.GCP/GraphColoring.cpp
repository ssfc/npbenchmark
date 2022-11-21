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

            Graph test_graph(input, seed);

            // create graph;
            /*
            int tmp;

            for (int i = 0;i < input.edgeNum; i++)
            {
                int v1 = input.edges[i][0];
                int v2 = input.edges[i][1];

                tmp = ++test_graph.vertex_edge[v1];
                test_graph.adj_list[v1][tmp - 1] = v2;
                tmp = ++test_graph.vertex_edge[v2];
                test_graph.adj_list[v2][tmp - 1] = v1;
            }
             */

            // cerr << "Finish creating graph." << endl;

            // test.print_graph();

            double start_time = clock();

            unsigned int test_iterations = test_graph.tabu_search();

            double end_time = clock();
            double elapsed_time = (end_time - start_time) / CLOCKS_PER_SEC;
            cerr << "success, iterations: " << test_iterations << " elapsed_time(s): " << elapsed_time
                 << " frequency:" << test_iterations / elapsed_time << endl;

            for (int i = 0;i < input.nodeNum;i++)
            {
                output[i] = test_graph.get_solution(i);
            }

            test_graph.free_memory();

            struct Record
            {
                unsigned int iterations;
                double elapsed_time;
                double frequency;
            };

            vector<Record> seed_record;

            // generate all results;
            for(seed=0;seed<5;seed++)
            {
                Graph this_graph(input, seed);

                start_time = clock();

                unsigned int this_iterations = this_graph.tabu_search();

                end_time = clock();
                elapsed_time = (end_time - start_time) / CLOCKS_PER_SEC;
                double frequency = this_iterations / elapsed_time;
                cerr << "success, iterations: " << this_iterations << " elapsed_time(s): " << elapsed_time
                     << " frequency:" << frequency << endl;

                Record this_record = {.iterations =  this_iterations, .elapsed_time =  elapsed_time, .frequency =  frequency};
                seed_record.push_back(this_record);

                this_graph.free_memory();
            }

            for(auto & i : seed_record)
            {
                cerr<<i.iterations<<" "<<i.elapsed_time<<" "<<i.frequency<<endl;
            }
            cerr<<endl;

        }
    };

    // solver.
    void solveGraphColoring(NodeColors& output, GraphColoring& input, std::function<bool()> isTimeout, int seed) {
        Solver().solve(output, input, isTimeout, seed);
    }
}
