#include "hea.h"

using namespace std;


// random number generator.
mt19937 pseudoRandNumGen;
void init_rand(int seed) { pseudoRandNumGen = mt19937(seed); }


Partition_Solution::Partition_Solution(int input_num_vertex, int input_num_color)
{
    num_vertex = input_num_vertex;
    solution.resize(input_num_vertex + 1, 0);
    partition.resize(input_num_color);
    for(auto & i : partition)
    {
        i.resize(input_num_vertex, 0);
    }
    partition_index.resize(input_num_vertex, 0);
    num_colors.resize(input_num_color, 0);
}


Partition_Solution::~Partition_Solution()
= default;

void Partition_Solution::print_population_solution()
{
    cerr << "Solution: ";
    for(int i=0;i<solution.size()-1;i++)
    {
        cerr << solution[i+1] << " ";
    }
    cerr << endl;

    cerr << "Partition index: ";
    for(unsigned int i : partition_index)
    {
        cerr << i << " ";
    }
    cerr << endl;

    cerr << "partition: " << endl;
    for(int i=0;i<partition.size();i++)
    {
        for(int j=0;j<num_colors[i];j++)
        {
            cerr << partition[i][j] << " ";
        }
        cerr << endl;
    }

    cerr << "color nums: ";
    for(int i : num_colors)
    {
        cerr << i << " ";
    }
    cerr << endl;
}


Population::Population(int input_num_population)
{
    min_conflict = INT_MAX;
    min_conflict_index = 0;
    num_conflict.resize(input_num_population);
}


Population::~Population()
= default;


Hybrid_Evolution::Hybrid_Evolution(int input_num_vertex, int input_num_color, int input_num_population, int input_seed)
{
    init_rand(input_seed);

    num_vertex = input_num_vertex;
    num_color = input_num_color;
    adj_list.resize(num_vertex);
    for (int i = 0; i < num_vertex; i++)
    {
        adj_list[i].resize(num_vertex, 0);
    }
    vertex_edge_num.resize(num_vertex, 0);

    conflict = 0;
    best_conflict = 0;

    adj_color_table.resize(input_num_vertex);
    for(auto & i : adj_color_table)
    {
        i.resize(input_num_color, 0);
    }

    tabu_tenure_table.resize(input_num_vertex);
    for(auto & i : tabu_tenure_table)
    {
        i.resize(input_num_color, 0);
    }

    moved = {-1, -1};
    equal_nontabu_delta.resize(2000, {-1, -1});
    equal_tabu_delta.resize(2000, {0,0});
    min_delta = 999999;
    iter = 0;
    max_iter = 1;

    num_population = input_num_population;
    solution_collection.resize(num_population);
    for (auto & i : solution_collection)
        i.resize(num_vertex + 1, 0);
    population_solution.resize(num_population, Partition_Solution(input_num_vertex, input_num_color));

    // debug variables;
    max_equal_nontabu_count = 0;
    max_equal_tabu_count = 0;
    start_time = 0;
    end_time = 0;
}


Hybrid_Evolution::~Hybrid_Evolution()
= default;


void Hybrid_Evolution::insert_adj_list(int v1, int v2)
{
    adj_list[v1][vertex_edge_num[v1]] = v2+1;
    vertex_edge_num[v1]++;

    adj_list[v2][vertex_edge_num[v2]] = v1+1;
    vertex_edge_num[v2]++;
}


void Hybrid_Evolution::find_move(vector<unsigned int> &solution)
{
    min_delta = 999999;
    int tabu_delta = 999999;
    int equal_nontabu_count = 0;
    int equal_tabu_count = 0;

    int aspiration = best_conflict - conflict;

    for (int i = 0; i < num_vertex; i++) // i is vertex;
    {
        unsigned int solution_i = solution[i+1]; // solution_i is color;

        if (adj_color_table[i][solution_i] > 0) // if vertex i overlap its neighbor's color;
        {
            for (int j = 0; j < num_color; j++) // j is color;
            {
                if (solution_i != j) // find a new color;
                {//cpu流水线
                    int this_delta = adj_color_table[i][j] - adj_color_table[i][solution_i]; // new-old, the less the better;
                    if (tabu_tenure_table[i][j] <= iter) //nontabu move;
                    {
                        if (this_delta < min_delta)
                        {//分支预判惩罚 6.0
                            equal_nontabu_count = 0;
                            min_delta = this_delta;

                            equal_nontabu_delta[equal_nontabu_count].u = i+1; // i is vertex;
                            equal_nontabu_delta[equal_nontabu_count].vj = j; // j is color;

                            equal_nontabu_count++;
                        }
                        else if (this_delta == min_delta)
                        {
                            equal_nontabu_delta[equal_nontabu_count].u = i+1; // i is vertex;
                            equal_nontabu_delta[equal_nontabu_count].vj = j; // j is color;

                            equal_nontabu_count++;
                        }

                        // debug part: estimate the size of max equal nontabu count;
                        if(max_equal_nontabu_count < equal_nontabu_count)
                        {
                            max_equal_nontabu_count = equal_nontabu_count;
                            // cerr << "max equal nontabu count: " << max_equal_nontabu_count << endl;
                        }
                        // debug part end;
                    }
                    else // tabu move;
                    {
                        if (this_delta < tabu_delta)
                        {//6.0
                            equal_tabu_count = 0;
                            tabu_delta = this_delta;

                            equal_tabu_delta[equal_tabu_count].u = i+1; // i is vertex;
                            equal_tabu_delta[equal_tabu_count].vj = j; // j is color;

                            equal_tabu_count++;
                        }
                        else if(this_delta == tabu_delta)
                        {
                            equal_tabu_delta[equal_tabu_count].u = i+1; // i is vertex;
                            equal_tabu_delta[equal_tabu_count].vj = j; // j is color;

                            equal_tabu_count++;
                        }

                        // debug part: estimate the size of max equal tabu count;
                        if(max_equal_tabu_count < equal_tabu_count)
                        {
                            max_equal_tabu_count = equal_tabu_count;
                            // cerr << "max equal tabu count: " << max_equal_tabu_count << endl;
                        }
                        // debug part end;
                    }
                }
            }
        }
    }

    if (tabu_delta < aspiration && tabu_delta < min_delta)
    {
        min_delta = tabu_delta;
        unsigned int rand_select = pseudoRandNumGen() % equal_tabu_count; // 相等tabu_delta随机选择
        // cerr << "random select tabu: " << rand_select << endl;
        moved = equal_tabu_delta[rand_select];
    }
    else
    {
        unsigned int rand_select = pseudoRandNumGen() % equal_nontabu_count; // 相等delta随机选择
        // cerr << "random select nontabu: " << rand_select << endl;
        moved = equal_nontabu_delta[rand_select];
    }
}


void Hybrid_Evolution::make_move(vector<unsigned int> &solution)
{
    conflict = min_delta + conflict; // update value of conflict;

    if (conflict < best_conflict)
        best_conflict = conflict; // update minimum conflict of history;

    unsigned int old_color = solution[moved.u];
    solution[moved.u] = moved.vj;
    tabu_tenure_table[moved.u-1][old_color] = iter + conflict + pseudoRandNumGen() % 10 + 1; //更新禁忌表

    // update adjacent color table;
    for (int i = 0; i < vertex_edge_num[moved.u-1]; i++)
    {
        int adj_list_node_moved_i = adj_list[moved.u-1][i];

        adj_color_table[adj_list_node_moved_i-1][old_color]--;
        adj_color_table[adj_list_node_moved_i-1][moved.vj]++;
    }
}


void Hybrid_Evolution::tabu_search(vector<unsigned int> &solution, bool is_limit)
{
    // compute initial conflict;
    for (int i = 0; i < num_vertex; i++)
    {
        int num_edge = vertex_edge_num[i];
        unsigned int this_vertex_color = solution[i+1];

        for (int j = 0; j < num_edge; j++)
        {
            unsigned int adj_color = solution[adj_list[i][j]];

            if (this_vertex_color == adj_color)
                conflict++;

            adj_color_table[i][adj_color]++; // initialize adjacent color table;
        }
    }

    conflict = conflict / 2;
    best_conflict = conflict;
    // cerr << "initial number of confilcts:" << conflict << endl;

    iter = 0;
    start_time = clock();

    if(is_limit) // set upper bound of iteration;
    {
        while (iter < max_iter && conflict > 0)
        {
            iter++;

            find_move(solution);
            make_move(solution);
        }
    }
    else
    {
        while (conflict > 0)
        {
            iter++;
            // cerr << "iter: " << iter << endl;

            if (iter % 1000000 == 0)
            {
                cerr << "Iteration: " << iter << " ";
                double elapsed_time = (clock() - start_time) / CLOCKS_PER_SEC;
                cerr << " elapsed time(s): " << elapsed_time
                     << " frequency:" << double(iter) / elapsed_time << endl;
            }

            find_move(solution);
            make_move(solution);
        }

        end_time = clock();

        double elapsed_time = (end_time - start_time) / CLOCKS_PER_SEC;
        cerr << "success, iterations: " << iter << " elapsed_time(s): " << elapsed_time
             << " frequency:" << double (iter) / elapsed_time << endl;

        // debug function:
        cerr << "max equal nontabu count: " << get_max_equal_nontabu_count() << endl;
        cerr << "max equal tabu count: " << get_max_equal_tabu_count() << endl;
    }
}


void Hybrid_Evolution::cross_over(unsigned int s1, unsigned int s2, vector<unsigned int>& index1)
{
    int A, B;
    Partition_Solution s[2] = {Partition_Solution(num_vertex, num_color), Partition_Solution(num_vertex, num_color)};
    s[0] = population_solution[s1];
    s[1] = population_solution[s2];

    for (int i = 0; i < num_color; i++) // 遍历所有颜色;
    {
        // 如果循环数是奇数，摘取s1的独立集；如果循环数是偶数，摘取s2的独立集。
        if (i % 2 == 0)
        {
            A = 0;
            B = 1;
        }
        else
        {
            A = 1;
            B = 0;
        }

        // 寻找抽取解中的包含顶点最多的颜色(最大独立集);
        int max_index;
        int max_num = -1;
        for (int j = 0; j < num_color; j++)
        {
            if (s[A].num_colors[j] > max_num)
            {
                max_index = j; // 抽取解中最大颜色的名称(序号);
                max_num = s[A].num_colors[j]; // 抽取解中最大颜色所包含的顶点数量;
            }
        }

        for (int j = 0; j<max_num; j++) // 遍历最多颜色的独立集;
        {
            int point = s[A].partition[max_index][j]; // 最多颜色独立集中的第j个顶点的名称; {2, 5, 6, 7, 10}
            index1[point] = i; // 将solution中的对应顶点point{2,5,6,7,10}改成颜色i; 为啥不是max_index? 因为伪代码就是这样;

            //j循环的过程中, 在B中删除这些点{2,5,6,7,10}
            unsigned int color = s[B].solution[point]; // 找出顶点{2,5,6,7,10}在B中的颜色;
            unsigned int partition_index = s[B].partition_index[point-1]; // 找出顶点{2,5,6,7,10}在分划B中的位置;

            // --s[B].color_num[color]; // 每删除顶点{2,5,6,7,10}中的一个, 就把顶点{2,5,6,7,10}在B中的颜色数量-1;
            s[B].partition[color][partition_index] = s[B].partition[color][--s[B].num_colors[color]]; // 把一个分划中末尾的顶点填补到删除顶点的位置;
            int t = s[B].partition[color][partition_index]; // 一个颜色分划中原先位于末尾, 现在填补到被删除顶点的顶点名字;
            s[B].partition_index[t-1] = partition_index; // 将替换到被删除顶点的顶点在分划中的位置更新为被删除顶点的位置;
        }

        //删除A中的这些点
        s[A].num_colors[max_index] = 0; // 将A中拥有最多颜色的置零; 其实也没有置零, 只是通过限制访问范围实现了”置零“
    }

    // 整理s[0](或者s[1])分化中剩下的顶点;
    for (int i = 0; i < num_color; i++)
    {
        int num = s[0].num_colors[i]; // s[0]中颜色i的数量; 其实这里选s[0]还是s[1]没有区别, 剩下的顶点是一样的;
        for (int j = 0; j<num; j++) // 遍历颜色i独立集中的剩余顶点;
        {
            int point = s[0].partition[i][j]; // 颜色i独立集中第j个顶点的名字;
            unsigned int color = pseudoRandNumGen() % num_color; //随机寻找一种颜色;
            index1[point] = color; // 给颜色i独立集中第j个顶点分配一种随机的颜色;
        }
    }
}


// debug function: compute conflict of a solution
int Hybrid_Evolution::compute_conflict(vector<unsigned int> &solution)
{
    int this_conflict = 0;
    //cerr <<endl;

    for (int i = 0; i < num_vertex; i++)
    {
        int num_edge = vertex_edge_num[i];
        unsigned int this_vertex_color = solution[i+1];

        for (int j = 0; j < num_edge; j++)
        {
            unsigned int adj_color = solution[adj_list[i][j]];

            if (this_vertex_color == adj_color)
                this_conflict++;
        }
    }

    this_conflict = this_conflict / 2;

    return this_conflict;
}


// debug function
long long int Hybrid_Evolution::get_iteration() const
{
    return iter;
}


// debug function: print adjacency list of graph;
void Hybrid_Evolution::print_adj_list() const
{
    cerr << "Adjacency list of graph: " << num_vertex << " " << num_color << endl;
    for (int i = 0; i < num_vertex; i++)
    {
        cerr << "Vertex " << i << ": ";
        for (int j = 0;j < vertex_edge_num[i];j++)
        {
            cerr << adj_list[i][j] << " ";
        }
        cerr << endl;
    }
}


// debug function:
int Hybrid_Evolution::get_max_equal_nontabu_count() const
{
    return max_equal_nontabu_count;
}

// debug function:
int Hybrid_Evolution::get_max_equal_tabu_count() const
{
    return max_equal_tabu_count;
}


/*
 * debugging command:
 * laptop: C:\wamp64\www\npbenchmark\npbenchmark-main\yuanFang_reduce
 * home server: /home/ssfc/yuanFang_reduce
 * g++ hea.cpp -g; gdb a.out
 * r 11 6 chvatal.txt
 * r 11 6 DSJC0250.9.txt
 * r 11 6 DSJC1000.9.txt
 */