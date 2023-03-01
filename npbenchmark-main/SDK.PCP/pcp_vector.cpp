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

    dbs_solution.reset(); // initialize solution all 0;
    best_solution.reset(); // initialize best solution all 0;
    prev_solution.reset(); // initialize prev solution all 0;
    vertex_weights.resize(num_vertex, 1);
    // print_vector("weight", weight);

    dbs_covered.reset(); // set dbs_covered all 0;
    dbs_uncovered.set(); // set dbs_uncovered all 1;

    tabu_tenure_table.resize(num_vertex, 0);
    // LINE 2:
    // tabu list TL <- NULL;
    // TL: tabu list;
    dbs_tabu_tenure_table.resize(num_vertex, 0);
    center_weights.resize(num_vertex, 0);
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

}

PCP_Vector::~PCP_Vector()
= default;

void PCP_Vector::greedy_construct()
{
    if(nodes_with_drops.empty())
    {
        for(int i=0;i<center_weights.size();i++)
        {
            center_weights[i] = center_cover_vertex[i].count();
        }
        print_vector("center_covered_weights: ", center_weights);

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
    vector<size_t> uncovered_vertices;
    for (size_t i = dbs_uncovered.find_first(); i != boost::dynamic_bitset<>::npos; i = dbs_uncovered.find_next(i))
    {
        uncovered_vertices.push_back(i);
    }

    size_t random_uncovered_index = generated_random() % uncovered_vertices.size();
    unsigned int random_uncovered_vertex = uncovered_vertices[random_uncovered_index];
    cerr << "random uncovered vertex: " << random_uncovered_vertex << endl;

    // LINE 5:
    // for j属于C do
    //    delta_j_prev <- delta_j,
    // delta_j_prev: previous move;
    // delta_j: current move;
    // j: 中心序号;
    // C: 中心集;
    // Meaning: backup before trial moves;
    prev_center_covered_weights = center_weights;

    // LINE 6:
    // for all i属于Cv do /* Cv: candidates covering v */
    // i: Cv中的中心序号;
    // Comment: 在前面的算法和表达式中, i用来表示顶点序号, 但是这里表示swap_in的中心序号;
    // Cv: 覆盖顶点v的中心集合;
    // print_index1("Cv list", vertex_reach_center[random_uncovered_vertex]);
    boost::dynamic_bitset<> Cv = vertex_reach_center[random_uncovered_vertex];
    cerr << "Cv list: " << endl;
    for (size_t i = Cv.find_first(); i != boost::dynamic_bitset<>::npos; i = Cv.find_next(i))
    {
        cerr << i << endl; // i is center name;
        try_open_center(i);
    }
}

void PCP_Vector::try_open_center(unsigned int center)
{
    // LINE 2:
    // for all v属于Vi do:
    // v: 顶点名称;
    // i: 中心序号; i在前面表示顶点序号, 这里却表示中心序号;
    // V(i): the set of vertex that center i can serve;
    // print_index1("Vi", center_cover_vertex[center]);
    boost::dynamic_bitset<> Vi = center_cover_vertex[center];
    cerr << "Vi" << ": ";
    for (size_t v = Vi.find_first(); v != boost::dynamic_bitset<>::npos; v = Vi.find_next(v))
    {
        // cerr << v << " ";
        // LINE 3:
        // if |X 交 Cv| = 1 then
        // X: current center set;
        // v: vertex
        // Cv: 覆盖顶点v的中心集合;
        // |X 交 Cv|: number of centers covering v in X;
        boost::dynamic_bitset<> Cv = vertex_reach_center[v];
        boost::dynamic_bitset<> intersection = dbs_solution & Cv;
        if (intersection.count() == 1)
        {
            cerr << "find one: ";
            print_index1("dbs solution", dbs_solution);
            print_index1("Cv", Cv);
        }
    }
    cerr << endl;
}


void PCP_Vector::vertex_weight_tabu_search()
{
    // LINE 1:
    // X <- init(G, p, rq);
    // X: initial solution generated;
    // G: graph;
    // p: centers;
    // rq: 最小化的最大服务半径;
    // Meaning: generates an initial solution X by a greedy algorithm; (2023年2月10日)
    greedy_construct();
    // random_construct();
    // print_index1("random construct solution", dbs_solution);

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


        while(dbs_uncovered.count()!=0 && iter<1)
        {
            cerr << "iteration: " << iter << endl;
            find_pair();

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
void PCP_Vector::get_solution(vector<NodeId>& output)
{
    int count = 0;
    for (size_t i = dbs_solution.find_first(); i != boost::dynamic_bitset<>::npos; i = dbs_solution.find_next(i))
    {
        output[count] = i;
        count++;
    }
}

void PCP_Vector::print_index1(const string& name, const boost::dynamic_bitset<>& dbs)
{
    cerr << name << ": ";
    for (size_t i = dbs.find_first(); i != boost::dynamic_bitset<>::npos; i = dbs.find_next(i))
    {
        cerr << i << " ";
    }

    cerr << endl;
}

// debug function: construct random solution;
void PCP_Vector::random_construct()
{
    size_t num_selected = 0;
    while (num_selected < num_center)
    {
        size_t random_index = generated_random() % num_vertex;
        if (!dbs_solution[random_index])
        {
            dbs_solution.set(random_index);
            num_selected++;
        }
    }
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
long long int PCP_Vector::get_iteration() const
{
    return iter;
}

// debug function:
void PCP_Vector::print_vector(const string& name, vector<unsigned int> &vect)
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


// (1) debug on laptop by clion:
// .\SDK_PCP.exe 999999 1 <C:\wamp64\www\npbenchmark\npbenchmark-main\SDK.PCP\data\pmed01.n100p005.txt >sln.pmed01.n100p005.txt
// (2) debug on laptop by g++ in command line:
// cd C:\wamp64\www\npbenchmark\npbenchmark-main\SDK.PCP
// g++ -static-libgcc -static-libstdc++ -I C:\boost_1_81_0 Main.cpp PCenter.cpp pcp_vector.cpp -O3 && .\a.exe 999999 1 <C:\wamp64\www\npbenchmark\npbenchmark-main\SDK.PCP\data\pmed01.n100p005.txt >sln.pmed01.n100p005.txt
// (3) debug on ubuntu gdb:
// g++ Main.cpp PCenter.cpp pcp_vector.cpp -g && gdb a.out
// r 999999 1 999999 1 <./data/pmed01.n100p005.txt >sln.txt

