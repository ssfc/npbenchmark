//
// Created by take_ on 2022/12/13.
//
# include "pcp_vector.h"

using namespace std;


PCP_Vector::PCP_Vector(int input_num_vertex, int input_num_center,
                       vector<vector<int>> &input_coverages, vector<vector<int>> &input_nodesWithDrops,
                       int input_seed)
                       :dbs_solution(input_num_vertex)
                       ,best_solution(input_num_vertex)
                       ,prev_solution(input_num_vertex)
                       ,dbs_covered(input_num_vertex)
                       ,dbs_uncovered(input_num_vertex)
                       ,moved{-1, -1}
{
    init_rand(input_seed); // initialize random generator;

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
        solution[i] = generated_random() % num_vertex;
    dbs_solution.reset(); // initialize solution all 0;
    best_solution.reset(); // initialize best solution all 0;
    prev_solution.reset(); // initialize prev solution all 0;
    weight.resize(num_vertex, 1);
    // print_vector("weight", weight);
    conflict = 0;
    best_conflict = 0;

    universe.resize(input_num_vertex, 0);
    covered.resize(input_num_vertex, 0);
    dbs_covered.reset(); // set dbs_covered all 0;
    uncovered.resize(input_num_vertex, 0);
    dbs_uncovered.set(); // set dbs_uncovered all 1;

    tabu_tenure_table.resize(num_vertex, 0);
    // LINE 2:
    // tabu list TL <- NULL;
    // TL: tabu list;
    dbs_tabu_tenure_table.resize(num_vertex, 0);
    min_delta = INT_MAX;
    center_covered_weights.resize(num_vertex, 0);
    prev_center_covered_weights.resize(num_vertex, 0);
    equal_delta.resize(2000, {0, 0});
    equal_pair.resize(2000, {0, 0});
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

void PCP_Vector::greedy_construct()
{
    if(nodes_with_drops.empty())
    {
        int equal_delta_in_construct[2000] = {0}; //非禁忌相同delta值
        int equal_count_in_construct = 0;
        while(dbs_solution.count()<num_center && dbs_covered.count()!=num_vertex) // do one iteration;
        {
            // cerr << "iteration: " << iter << endl;

            unsigned long long dbs_max_overlap_size = 0;
            int dbs_max_overlap_index = 0;

            // cerr << "dbs_uncovered" << dbs_uncovered << endl;
            for(int j=0;j<num_vertex;j++) // consider only one set;
            {
                boost::dynamic_bitset<> this_intersection = center_cover_vertex[j] & dbs_uncovered;
                unsigned long long this_intersection_size = this_intersection.count();

                if(this_intersection_size > dbs_max_overlap_size)
                {
                    dbs_max_overlap_size = this_intersection_size;
                    equal_count_in_construct = 0;
                    equal_delta_in_construct[equal_count_in_construct] = j; // j is index of center;
                    equal_count_in_construct++;
                }
                else if(this_intersection_size == dbs_max_overlap_size)
                {
                    equal_delta_in_construct[equal_count_in_construct] = j; // j is index of center;
                    equal_count_in_construct++;
                }
            }

            // cerr << "equal count in construct: " << equal_count_in_construct << endl;
            unsigned int dbs_rand_select = generated_random() % equal_count_in_construct; // 相等tabu_delta随机选择
            // cerr << "dbs random select: " << dbs_rand_select << endl;
            // cerr << "dbs random select index: " << equal_delta_in_construct[dbs_rand_select] << endl;

            dbs_solution.set(equal_delta_in_construct[dbs_rand_select]);

            dbs_covered = dbs_covered | center_cover_vertex[equal_delta_in_construct[dbs_rand_select]];
            // cerr << "DBS Cover after union size (" << dbs_covered.count() << "): " << endl;
            // print_index1("", dbs_covered);

            dbs_uncovered = ~dbs_covered;

            // print_index1("DBS Uncover after union are: ", dbs_uncovered);

            iter++;
        }

        print_index1("DBS Center selected are: ", dbs_solution);
    }

    // LINE 2:
    // iter <- 1;
    // iter: current iteration;
    iter = 0;
}

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
            vector<int> temp; // dim, num_vertex;
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

    unsigned int rand_select = generated_random() % equal_count; // equal_delta随机选择
    moved = equal_delta[rand_select];
}

void PCP_Vector::find_pair()
{
    // LINE 2:
    // The set of best swap moves M <- NULL;
    // M: the set of best swap moves;
    fill(equal_pair.begin(), equal_pair.end(), Move{0,0});
    // print_equal_pair();

    // LINE 3:
    // The best objective value obj <- +INF;
    // Meaning: objective value should be optimized to zero, so start with infinity; (2023年2月19日)
    int obj = INT_MAX;

    // LINE 4:
    // v <- a randomly picked uncovered vertex in U(X);
    print_index1("uncovered: ", dbs_uncovered);
    vector<size_t> uncovered_indices;
    for (size_t i = dbs_uncovered.find_first(); i != boost::dynamic_bitset<>::npos; i = dbs_uncovered.find_next(i))
    {
        uncovered_indices.push_back(i);
    }
    size_t random_uncovered_index = generated_random() % uncovered_indices.size();
    cerr << "random select index: " << uncovered_indices[random_uncovered_index] << endl;

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
    // LINE 1:
    // X <- init(G, p, rq);
    // X: initial solution generated;
    // G: graph;
    // p: centers;
    // rq: 最小化的最大服务半径;
    // Meaning: generates an initial solution X by a greedy algorithm; (2023年2月10日)
    greedy_construct();

    if(nodes_with_drops.empty())
    {
        // LINE 2:
        // X* <- X
        // X*: history best solution;
        // X: initial solution generated (a set of centers);
        best_solution = dbs_solution;
        print_index1("best_solution", best_solution);

        // X_prev <- X;
        // X_prev: solution of the previous iteration;
        // X: initial solution generated;
        prev_solution = dbs_solution;
        print_index1("prev_solution", prev_solution);

        conflict = count(covered.begin(), covered.end(), 0);
        best_conflict = conflict;
        cerr << "number of uncovered in the initial solution: " << conflict << endl;

        while(conflict!=0 && iter<1)
        {
            cerr << "iteration: " << iter << endl;
            find_move();
            find_pair();
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

void PCP_Vector::print_index1(string name, boost::dynamic_bitset<> dbs)
{
    cerr << name << ": ";
    for (size_t i = dbs.find_first(); i != boost::dynamic_bitset<>::npos; i = dbs.find_next(i))
    {
        cerr << i << " ";
    }

    cerr << endl;
}

// debug function;
void PCP_Vector::print_tabu_tenure_table()
{
    cerr << "tabu tenure table: " << endl;
    for(int i=0;i<num_vertex;i++)
    {
        cerr << tabu_tenure_table[i] << " ";
    }
    cerr << endl;
}

// debug function;
long long int PCP_Vector::get_iteration()
{
    return iter;
}

// debug function:
void PCP_Vector::print_vector(string name, vector<unsigned int> &vect)
{
    cerr << name << ": ";
    for(unsigned int i : vect)
    {
        cerr << i << " ";
    }
    cerr << endl;
}

// debug function:
void PCP_Vector::print_equal_pair()
{
    cerr << "Equal pair: ";
    for(Move i : equal_pair)
    {
        cerr << "{" << i.center_in << ", ";
        cerr << i.center_out << "}";
    }
    cerr << endl;
}


// debug on laptop by clion:
// .\SDK_PCP.exe 999999 1 <C:\wamp64\www\npbenchmark\npbenchmark-main\SDK.PCP\data\pmed01.n100p005.txt >sln.pmed01.n100p005.txt
// debug on laptop by g++ in command line:
// cd C:\wamp64\www\npbenchmark\npbenchmark-main\SDK.PCP
// g++ -static-libgcc -static-libstdc++ -I C:\boost_1_81_0 Main.cpp PCenter.cpp pcp_vector.cpp -O3 && .\a.exe 999999 1 <C:\wamp64\www\npbenchmark\npbenchmark-main\SDK.PCP\data\pmed01.n100p005.txt >sln.pmed01.n100p005.txt
