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


            int num_population = 1;
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
                for(auto& x : test.adj_color_table) memset(&x[0],0,sizeof(x[0])*x.size());
                for(auto& x : test.tabu_tenure_table) memset(&x[0],0,sizeof(x[0])*x.size());

                test.conflict = 0;
                test.best_conflict = 0;

                // initialization: set random solution to each solution in the population;
                for (int i = 1; i <= test.num_vertex; i++)
                {
                    test.solution_collection[p][i] = pseudoRandNumGen() % test.num_color;
                    //cerr << solution[i] <<' ';
                }

                // do tabu-search for each population in the collection;
                // cerr << "Conflict before tabu search is: " << test.compute_conflict(test.solution_collection[p]) << endl;
                test.tabu_search(test.solution_collection[p], true);
                // cerr << "Conflict after tabu search is: " << test.compute_conflict(test.solution_collection[p]) << endl;

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

            for(auto & i : test.population_solution)
            {
                for(auto& x : i.psol) memset(&x[0],0,sizeof(x[0])*x.size());
                memset(&i.color_num[0], 0, sizeof(i.color_num[0]) * i.color_num.size());
                memset(&i.index1s[0], 0, sizeof(i.index1s[0]) * i.index1s.size());
                memset(&i.index2s[0], 0, sizeof(i.index2s[0]) * i.index2s.size());
            }

            double start_time = clock();

            for (int p = 0; p < num_population; p++) {
                for (int i = 1; i <= test.num_vertex; i++) // i is name of vertex;
                {
                    // copy color solution from solution_collection to population_solution[p].index1s;
                    test.population_solution[p].index1s[i] = test.solution_collection[p][i];
                    unsigned int color = test.solution_collection[p][i]; // take out the color of solution[p][i];
                    int color_num = test.population_solution[p].color_num[color]; // take out the color num of solution[p][i] corresponding color;

                    // {[p][i]的颜色, [p][i]的颜色数量} = 顶点; 将某颜色的独立集成员顶点按顺序排列, 范围之外的置零;
                    test.population_solution[p].psol[color][color_num] = i;
                    // 顶点i在所属颜色独立集中的序号;
                    test.population_solution[p].index2s[i] = color_num++;
                    test.population_solution[p].color_num[color] = color_num; // 解[p][i]对应的颜色独立集magnitude+1;
                }

                // for debugging:
                test.population_solution[p].print_population_solution();
            }

            //////////////////////////////////////// 前面的都看懂了
            /* to reduce
            Population_solution temps(input.nodeNum, input.colorNum);

            long long int population_iteration = 0;
            while (population.min_conflict != 0)
            {
                // random select two index from population as parents;
                unsigned int p1 = pseudoRandNumGen() % num_population, p2;

                do
                {
                    p2 = pseudoRandNumGen() % num_population;
                } while (p1 == p2);

                for(auto& x : temps.psol) memset(&x[0],0,sizeof(x[0])*x.size());
                memset(&temps.color_num[0], 0, sizeof(temps.color_num[0]) * temps.color_num.size());
                memset(&temps.index1s[0], 0, sizeof(temps.index1s[0]) * temps.index1s.size());
                memset(&temps.index2s[0], 0, sizeof(temps.index2s[0]) * temps.index2s.size());
                // cerr << "After 2: " << temps.color_num[17] << endl; // debug memset sentence;

                test.cross_over(p1, p2, temps.index1s);

                // reset adj_color_table and tabu_tenure_table to zero;
                for(auto& x : test.adj_color_table) memset(&x[0],0,sizeof(x[0])*x.size());
                for(auto& x : test.tabu_tenure_table) memset(&x[0],0,sizeof(x[0])*x.size());

                test.conflict = 0;
                test.best_conflict = 0;

                test.tabu_search(temps.index1s, true); // 仅仅需要对新形成的temps进行禁忌搜索;

                for (int i = 1; i <= test.num_vertex; i++)
                {//变成划分的形式
                    unsigned int color = temps.index1s[i];
                    int color_num = temps.color_num[color];
                    temps.psol[color][color_num] = i;
                    temps.index2s[i] = color_num;
                    temps.color_num[color] = ++color_num;
                }

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

                if (test.conflict < population.min_conflict)
                {
                    population.min_conflict = test.conflict;
                    population.min_conflict_index = max_conflict_index;
                }

                if(population_iteration % 20 == 0)
                {
                    cerr << "Population iteration: " << population_iteration << endl;
                    cerr << "min conflict: " << population.min_conflict << endl;
                    cerr << "min conflict index: " << population.min_conflict_index << endl;
                }

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
                for(int i=1;i<=test.num_vertex;i++)
                {
                    cerr << test.population_solution[population.min_conflict_index].index1s[i] << " ";
                }
                cerr << endl;

                cerr << "conflict of solution 19: ";
                cerr << test.compute_conflict(test.population_solution[19].index1s) << endl;

                cerr << "conflict of final solution: ";
                cerr << test.compute_conflict(test.population_solution[population.min_conflict_index].index1s) << endl;
            }
            else
                cerr << "over time" << endl;


            for (int i = 0;i < input.nodeNum;i++)
            {
                output[i] = test.population_solution[population.min_conflict_index].index1s[i+1];
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
             */// to reduce
        }
    };

    // solver.
    void solveGraphColoring(NodeColors& output, GraphColoring& input, std::function<bool()> isTimeout, int seed) {
        Solver().solve(output, input, isTimeout, seed);
    }
}