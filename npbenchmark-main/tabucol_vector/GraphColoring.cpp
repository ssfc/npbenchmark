#include "GraphColoring.h"
#include "tabucol.h"

#include <climits>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
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

            /*
            for (NodeId n = 0; !isTimeout() && (n < input.nodeNum); ++n)
            {
                output[n] = rand(input.colorNum);
            }
            */


            Graph test_graph(input.nodeNum, input.edgeNum, input.colorNum, input.edges, seed);

            // create graph;
            // cerr << "Finish creating graph." << endl;
            // test.print_graph();

            test_graph.tabu_search();

            for (int i = 0;i < input.nodeNum;i++)
            {
                output[i] = test_graph.get_solution(i);
            }


            /*
            bool is_multiple_seed = true;
            struct Record
            {
                unsigned int iterations;
                double elapsed_time;
                double frequency;
            };
            vector<Record> seed_record;
            // generate all results;
            for(int i=1;i<=seed;i++)
            {
                cerr << "current seed:" << i << endl;
                Graph this_graph(input, i);
                start_time = clock();
                unsigned int this_iterations = this_graph.tabu_search();
                end_time = clock();
                elapsed_time = (end_time - start_time) / CLOCKS_PER_SEC;
                double frequency = this_iterations / elapsed_time;
                cerr << "success, iterations: " << this_iterations << " elapsed_time(s): " << elapsed_time
                     << " frequency:" << frequency << endl;
                Record this_record = {.iterations =  this_iterations, .elapsed_time =  elapsed_time, .frequency =  frequency};
                seed_record.push_back(this_record);
            }
            ofstream myfile;
            myfile.open ("seed_record.md");
            myfile << "| seed | iterations | elapsed_time | frequency | \n";
            myfile << "| ---- | ---- | ---- | ---- | \n";
            vector<double> collection_iterations;
            vector<double> collection_elapsed_time;
            vector<double> collection_frequency;
            for(int i=0;i<seed_record.size();i++)
            {
                cerr<<seed_record[i].iterations<<" "<<seed_record[i].elapsed_time<<" "<<seed_record[i].frequency<<endl;
                myfile << "|" << i+1 << "|" << seed_record[i].iterations << "|" << seed_record[i].elapsed_time
                       << "|" <<seed_record[i].frequency << "| \n";
                collection_iterations.push_back(seed_record[i].iterations);
                collection_elapsed_time.push_back(seed_record[i].elapsed_time);
                collection_frequency.push_back(seed_record[i].frequency);
            }
            cerr<<endl;
            double sum_iterations = accumulate(collection_iterations.begin(), collection_iterations.end(), 0.0);
            double mean_iterations = sum_iterations / double (seed_record.size());
            cerr << "mean iterations: " << mean_iterations <<endl;
            double square_sum_iterations = inner_product(collection_iterations.begin(), collection_iterations.end(), collection_iterations.begin(), 0.0);
            double stdev_iterations = sqrt(square_sum_iterations / double (seed_record.size()) - mean_iterations * mean_iterations);
            cerr << "stdev iterations: " << stdev_iterations <<endl;
            double sum_elapsed_time = accumulate(collection_elapsed_time.begin(), collection_elapsed_time.end(), 0.0);
            double mean_elapsed_time = sum_elapsed_time / double (seed_record.size());
            cerr << "mean elapsed_time: " << mean_elapsed_time <<endl;
            double square_sum_elapsed_time = inner_product(collection_elapsed_time.begin(), collection_elapsed_time.end(), collection_elapsed_time.begin(), 0.0);
            double stdev_elapsed_time = sqrt(square_sum_elapsed_time / double (seed_record.size()) - mean_elapsed_time * mean_elapsed_time);
            cerr << "stdev elapsed_time: " << stdev_elapsed_time <<endl;
            double sum_frequency = accumulate(collection_frequency.begin(), collection_frequency.end(), 0.0);
            double mean_frequency = sum_frequency / double (seed_record.size());
            cerr << "mean frequency: " << mean_frequency <<endl;
            double square_sum_frequency = inner_product(collection_frequency.begin(), collection_frequency.end(), collection_frequency.begin(), 0.0);
            double stdev_frequency = sqrt(square_sum_frequency / double (seed_record.size()) - mean_frequency * mean_frequency);
            cerr << "stdev frequency: " << stdev_frequency <<endl;
            myfile << "|Mean|" << mean_iterations << "|" << mean_elapsed_time << "|" << mean_frequency << "| \n";
            myfile << "|Stdev|" << stdev_iterations << "|" << stdev_elapsed_time << "|" << stdev_frequency << "| \n";
             */

        }
    };

    // solver.
    void solveGraphColoring(NodeColors& output, GraphColoring& input, std::function<bool()> isTimeout, int seed) {
        Solver().solve(output, input, isTimeout, seed);
    }
}