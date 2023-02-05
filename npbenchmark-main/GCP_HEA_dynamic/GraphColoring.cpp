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


            int num_population = 10;
            Population population(num_population);

            Hybrid_Evolution test(input.nodeNum, input.colorNum, num_population, seed);

            for (int i = 0;i < input.edgeNum; i++)
            {
                int v1 = input.edges[i][0];
                int v2 = input.edges[i][1];

                test.insert_adj_list(v1, v2);
            }
            // test.print_adj_yf_list();
            // test.print_adj_list();

            /* to debug
            vector<unsigned int> temp_solution;
            temp_solution.resize(test.num_vertex+1, 0);
            // initialization: set random solution to each solution in the population;
            for (int i = 1; i <= test.num_vertex; i++)
            {
                temp_solution[i] = pseudoRandNumGen() % test.num_color;
                //cerr << solution[i] <<' ';
            }
            cerr << "Conflict before tabu search is: " << test.compute_conflict(temp_solution) << endl;
            // cerr << "iterations: " << test.get_iteration() << endl;
            double start_time = clock();
            // test.tabu_yf_search(temp_solution_1, false);
            test.tabu_search(temp_solution, true);
            double end_time = clock();
            double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
            // cerr << "elapsed time(s): " << elapsed_time << endl;
            cerr << "Iterations: " << test.get_iteration() << " elapsed_time(s): " << elapsed_time
                 << " frequency:" << double (test.get_iteration()) / elapsed_time << endl;
            cerr << "Conflict after tabu search is: " << test.compute_conflict(temp_solution) << endl;
            // cerr << "iterations: " << test.get_iteration() << endl;
             */// to debug

            // this is also the process of initialization;
            for (int p = 0; p < num_population; p++)
            {
                for (int i = 0; i < test.num_vertex; i++)
                {
                    memset(test.adj_color_table[i], 0, test.num_color * sizeof(int));
                    memset(test.tabu_tenure_table[i], 0, test.num_color * sizeof(long long int));
                }

                test.conflict = 0;
                test.best_conflict = 0;

                // initialization: set random color to each sol in the population;
                for (int i = 0; i < test.num_vertex; i++)
                {
                    test.solution_collection[p][i] = pseudoRandNumGen() % test.num_color;
                }

                // do tabu-search for each population in the collection;
                // cerr << "Population: " << p << endl;
                // cerr << "Solution before tabu search: ";
                // for(int i=0;i<input.nodeNum;i++)
                // {
                //    cerr << test.solution_collection[p][i] << " ";
                // }
                // cerr << endl;
                // cerr << "Conflict before tabu search: " << test.compute_conflict(test.solution_collection[p]) << endl;

                test.tabu_search(test.solution_collection[p], true);
                // cerr << "Solution after tabu search:  ";
                // for(int i=0;i<input.nodeNum;i++)
                // {
                //    cerr << test.solution_collection[p][i] << " ";
                // }
                // cerr << endl;
                // cerr << "Conflict after tabu search is:  " << test.compute_conflict(test.solution_collection[p]) << endl;

                population.num_conflict[p] = test.conflict;

                // record the min conflict up till now;
                if (test.conflict < population.min_conflict)
                {
                    population.min_conflict = test.conflict;
                    population.min_conflict_index = p;
                }

                if (test.conflict == 0)
                    break;
            }

            double start_time = clock();

            ///* to reduce
            // construct partition for each solution in the solution;
            for (int p = 0; p < num_population; p++)
            {
                // cerr << "p: " << p <<" " << endl;
                for (int i = 0; i < test.num_vertex; i++) // i is name of vertex;
                {
                    // copy color solution from solution_collection to population_solution[p].index1s;
                    test.population_solution[p].solution[i] = test.solution_collection[p][i];
                }
                test.population_solution[p].construct_partition();

                // for debugging:
                // test.population_solution[p].print_population_solution();
            }


            Partition_Solution temps(input.nodeNum, input.colorNum);

            long long int population_iteration = 0;
            while (population.min_conflict != 0)
            {
                // random select two index from population as parents;
                unsigned int p1 = pseudoRandNumGen() % num_population, p2;
                do
                {
                    p2 = pseudoRandNumGen() % num_population;
                } while (p1 == p2);

                // 因为num_colors定义着边界, 所以只有num_colors需要置零;
                memset(&temps.num_colors[0], 0, sizeof(temps.num_colors[0]) * temps.num_colors.size());
                // cerr << "After 2: " << temps.color_num[17] << endl; // debug memset sentence;

                // debug information
                // cerr << "p1 before cross over: " << endl;
                // test.population_solution[p1].print_population_solution();
                // cerr << "p2 before cross over: " << endl;
                // test.population_solution[p2].print_population_solution();

                test.cross_over(p1, p2, temps.solution);

                // debug information;
                // cerr << "solution generated by cross over: ";
                // for(int i=0;i<input.nodeNum;i++)
                // {
                //     cerr << temps.solution[i] << " ";
                // }
                // cerr << endl;

                // reset adj_color_table and tabu_tenure_table to zero;
                for (int i = 0; i < test.num_vertex; i++)
                {
                    memset(test.adj_color_table[i], 0, test.num_color * sizeof(int));
                    memset(test.tabu_tenure_table[i], 0, test.num_color * sizeof(long long int));
                }


                test.conflict = 0;
                test.best_conflict = 0;

                test.tabu_search(temps.solution, true); // 仅仅需要对新形成的temps进行禁忌搜索;

                // 由temps的index1s构造出partition, index2s, color_num;
                temps.construct_partition();

                // cerr << "temps structure: " << endl;
                // temps.print_population_solution();

                //////////////////////////////////////// 前面的都验证了;
                // 找出种群中的最大冲突数;
                int max_conflict = -1, max_conflict_index;
                for (int i = 0; i < num_population; i++)
                {
                    if (population.num_conflict[i] > max_conflict)
                    {
                        max_conflict = population.num_conflict[i];
                        max_conflict_index = i;
                    }
                }

                test.population_solution[max_conflict_index] = temps; // 将种群中冲突数最大的替换成temps
                population.num_conflict[max_conflict_index] = test.conflict;

                // 因为只有交叉后的结果是新来的, 所以只需比较交叉后的结果和原种群中最小即可;
                if (test.conflict < population.min_conflict)
                {
                    population.min_conflict = test.conflict;
                    population.min_conflict_index = max_conflict_index;
                }

                ///*
                if(population_iteration % 50 == 0)
                {
                    cerr << "Population iteration: " << population_iteration << endl;
                    cerr << "min conflict: " << population.min_conflict << endl;
                    cerr << "min conflict index: " << population.min_conflict_index << endl;
                }
                //*/

                population_iteration++;
            }


            double end_time = clock();
            double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
            cerr << "Population iteration: " << population_iteration << " ";
            cerr << "elapsed time(s): " << elapsed_time << " ";
            cerr << "Population frequency: " << double (population_iteration) / elapsed_time << endl;

            if (population.min_conflict == 0)
            {
                cerr << "color of each vertex: ";
                for(int i=0;i<test.num_vertex;i++)
                {
                    cerr << test.population_solution[population.min_conflict_index].solution[i] << " ";
                }
                cerr << endl;

                cerr << "conflict of solution 0: ";
                cerr << test.compute_conflict(test.population_solution[0].solution) << endl;

                cerr << "conflict of final solution: ";
                cerr << test.compute_conflict(test.population_solution[population.min_conflict_index].solution) << endl;
            }
            else
                cerr << "over time" << endl;


            for (int i = 0;i < input.nodeNum;i++)
            {
                output[i] = test.population_solution[population.min_conflict_index].solution[i];
            }

            // debug function:
            cerr << "output: " << endl;
            for(int i=0;i<input.nodeNum;i++)
            {
                {
                    cerr << output[i] << " ";
                }
            }
            cerr << endl;
            //*/// to reduce
        }
    };

    // solver.
    void solveGraphColoring(NodeColors& output, GraphColoring& input, std::function<bool()> isTimeout, int seed) {
        Solver().solve(output, input, isTimeout, seed);
    }
}