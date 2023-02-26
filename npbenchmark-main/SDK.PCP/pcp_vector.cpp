//
// Created by take_ on 2022/12/13.
//
# include "pcp_vector.h"

using namespace std;



PCP_Vector::PCP_Vector(int input_num_vertex, int input_num_center,
                       vector<vector<int>> &input_coverages, vector<vector<int>> &input_nodesWithDrops,
                       int input_seed)
                       :dbs_covered(input_num_vertex)
                       ,moved{-1, -1}
{
    initRand(input_seed); // initialize random generator;

    num_vertex = input_num_vertex;
    num_center = input_num_center;

    center_coverages.resize(input_num_vertex);
    for(int i=0;i<input_coverages.size();i++)
    {
        center_coverages[i].resize(input_num_vertex);
        for(int j=0;j<input_coverages[i].size();j++)
        {
            int index = input_coverages[i][j];
            center_coverages[i][index] = 1;
        }
    }

    center_cover_vertex.resize(input_num_vertex, boost::dynamic_bitset<>(input_num_vertex));
    vertex_reach_center.resize(input_num_vertex, boost::dynamic_bitset<>(input_num_vertex));
    for(int i=0;i<input_coverages.size();i++) // i is center name;
    {
        for(int j=0;j<input_coverages[i].size();j++) // j is vertex name;
        {
            int index = input_coverages[i][j];
            center_cover_vertex[i].set(index, true);
            vertex_reach_center[index].set(i, true);
        }
        // cerr << "center_cover_vertex[" << i << "] " << center_cover_vertex[i] << endl;
    }
    // cerr << endl;

    // cerr << "center_cover_vertex[0]: " << center_cover_vertex[0] << endl;
    // cerr << "center_cover_vertex[99]: " << center_cover_vertex[99] << endl;
    // cerr << "vertex_reach_center[0]: " << vertex_reach_center[0] << endl;

    nodes_with_drops.resize(input_nodesWithDrops.size());
    for(int i=0;i<input_nodesWithDrops.size();i++)
    {
        for(int j=0;j<input_nodesWithDrops[i].size();j++)
        {
            nodes_with_drops[i].push_back(input_nodesWithDrops[i][j]);
        }
    }

    solution.resize(num_center, 0);
    for (int i = 0; i < num_center; i++)
        solution[i] = pseudoRandNumGen() % num_vertex;
    conflict = 0;
    best_conflict = 0;

    universe.resize(input_num_vertex, 0);
    covered.resize(input_num_vertex, 0);
    dbs_covered.reset(); // set dbs_covered all 0;
    uncovered.resize(input_num_vertex, 0);
    dbs_uncovered.set(); // set dbs_uncovered all 1;

    tabu_tenure_table.resize(num_vertex, 0);
    min_delta = INT_MAX;
    equal_delta.resize(2000, {0, 0});
    iter = 0;

    // debug variables;
    start_time = clock();

    // compute initial conflict of random solution;
    // 62 59 98 41 77
    /*
    solution[0] = 62;
    solution[1] = 59;
    solution[2] = 98;
    solution[3] = 41;
    solution[4] = 77;
     */

    for(int i=0;i<num_center;i++)
    {
        for(int j=0;j<covered.size();j++)
        {
            covered[j] += center_coverages[solution[i]][j];
        }
    }
}

PCP_Vector::~PCP_Vector()
= default;

void PCP_Vector::swap_center()
{
    for(int i=0;i<covered.size();i++)
    {
        covered[i] += center_coverages[moved.center_in][i];
    }

    for(int i=0;i<covered.size();i++)
    {
        covered[i] -= center_coverages[moved.center_out][i];
    }
}

void PCP_Vector::find_move()
{
    min_delta = INT_MAX;

    int equal_count = 0;

    vector<int> solution_vector; // make solution as 001010, in which solution member is 1's index;
    vector<int> set_selected;
    vector<int> set_unselected;

    solution_vector.resize(num_vertex);
    for(int i=0;i<num_center;i++)
    {
        solution_vector[solution[i]] = 1;
    }

    for(int i=0;i<solution_vector.size();i++)
    {
        if(solution_vector[i]==0)
        {
            set_unselected.push_back(i);
        }
        else
        {
            set_selected.push_back(i);
        }
    }

    for(int i : set_selected) // center out;
    {
        for(int j : set_unselected) // center in;
        {
            vector<int> temp; // dim, num_node;
            temp.resize(covered.size());
            temp.assign(covered.begin(), covered.end());

            for(int k=0;k<temp.size();k++)
            {
                temp[k] += center_coverages[j][k];
            }

            for(int k=0;k<temp.size();k++)
            {
                temp[k] -= center_coverages[i][k];
            }

            int this_delta = count(temp.begin(), temp.end(), 0) - conflict;
            if (tabu_tenure_table[i] <= iter && tabu_tenure_table[j] <= iter) //非禁忌移动;
            {
                if(this_delta < min_delta) // the less the better;
                {
                    equal_count = 0;
                    min_delta = this_delta;

                    equal_delta[equal_count].center_out = i; // i is center out;
                    equal_delta[equal_count].center_in = j; // j is center in;

                    equal_count++;
                }
                else if(this_delta == min_delta) // the less the better;
                {
                    equal_delta[equal_count].center_out = i; // i is center out;
                    equal_delta[equal_count].center_in = j; // j is center in;

                    equal_count++;
                }
            }
        }
    }

    unsigned int rand_select = pseudoRandNumGen() % equal_count; // equal_delta随机选择
    moved = equal_delta[rand_select];
}

void PCP_Vector::make_move()
{
    for(int i=0;i<num_center;i++)
    {
        if(solution[i] == moved.center_out)
        {
            solution[i] = moved.center_in;
        }
    }
    swap_center();
    conflict = min_delta + conflict;
    if (conflict < best_conflict)
        best_conflict = conflict; // update minimum conflict of history;

    tabu_tenure_table[moved.center_out] = iter + 1; //更新禁忌表
    tabu_tenure_table[moved.center_in] = iter + 1; //更新禁忌表
}

void PCP_Vector::local_search()
{
    if(nodes_with_drops.empty())
    {
        conflict = count(covered.begin(), covered.end(), 0);
        best_conflict = conflict;
        cerr << "number of uncovered in the initial solution: " << conflict << endl;

        while(conflict!=0)
        {
            cerr << "iteration: " << iter << endl;
            find_move();
            make_move();

            ///* debug: tabu tenure;
            cerr << "tabu tenure out: " << tabu_tenure_table[moved.center_out] << endl;
            cerr << "tabu tenure in: " << tabu_tenure_table[moved.center_in] << endl;
            // print_tabu_tenure_table();
             //*/

            iter++;
        }

        // print final tabu_tenure_table;
        // print_tabu_tenure_table();

        double elapsed_time = (clock() - start_time) / CLOCKS_PER_SEC;
        cerr << "success, iterations: " << iter << " elapsed_time(s): " << elapsed_time
             << " frequency:" << double (iter) / elapsed_time << endl;
    }
}

// get solution;
unsigned int PCP_Vector::get_solution(int i)
{
    return solution[i];
}

// debug function;
void PCP_Vector::print_tabu_tenure_table()
{
    cerr << "tabu tenure table: " << endl;
    for(int i=0;i<num_vertex;i++)
    {
        for(int j=0;j<num_vertex;j++)
        {
            cerr << tabu_tenure_table[i] << " ";
        }
        cerr << endl;
    }
}

// debug function;
long long int PCP_Vector::get_iteration()
{
    return iter;
}


