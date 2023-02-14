#include "head.h"

using namespace std;


// random number generator.
mt19937 pseudoRandNumGen;
void init_rand(int seed) { pseudoRandNumGen = mt19937(seed); }


Solution_Partition::Solution_Partition(int input_num_vertex, int input_num_color)
{
    num_vertex = input_num_vertex;
    solution.resize(input_num_vertex, 0);
    partition.resize(input_num_color);
    for(auto & i : partition)
    {
        i.resize(input_num_vertex, 0);
    }
    partition_index.resize(input_num_vertex, 0);
    num_colors.resize(input_num_color, 0);
}

void Solution_Partition::construct_partition()
{
    for (int i = 0; i < num_vertex; i++)
    {
        unsigned int color = solution[i];
        int color_num = num_colors[color];
        partition[color][color_num] = i;
        partition_index[i] = color_num;
        num_colors[color] = ++color_num;
    }
}

Solution_Partition::~Solution_Partition()
= default;

void Solution_Partition::print_solution_partition()
{
    cerr << "Solution: ";
    for(unsigned int i : solution)
    {
        cerr << i << " ";
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


Hybrid_Evolution::Hybrid_Evolution(int input_num_vertex, int input_edge_num, int input_num_color,
                                   vector<array<int, 2>>& input_edges, int input_seed)
        :p1(input_num_vertex, input_num_color)
        ,p2(input_num_vertex, input_num_color)
        ,elite1(input_num_vertex, input_num_color)
        ,elite2(input_num_vertex, input_num_color)
        ,best_solution(input_num_vertex, input_num_color)
{
    init_rand(input_seed);

    num_vertex = input_num_vertex;
    num_color = input_num_color;
    adj_list = new int* [num_vertex];
    for (int i = 0; i < num_vertex; i++)
    {
        adj_list[i] = new int[num_vertex];
        int* adj_list_i = adj_list[i];
        for (int j = 0; j < num_vertex; j++)
        {
            adj_list_i[j] = 0;
        }
    }
    vertex_edge_num.resize(num_vertex, 0);

    conflict = 0;
    best_history_conflict = 0;

    adj_color_table = new int* [num_vertex];
    for (int i = 0; i < num_vertex; i++)
    {
        adj_color_table[i] = new int[num_color];
        int* adj_color_table_i = adj_color_table[i];
        for (int j = 0; j < num_color; j++)
        {
            adj_color_table_i[j] = 0;
        }
    }

    tabu_tenure_table = new long long int* [num_vertex];
    for (int i = 0; i < num_vertex; i++)
    {
        tabu_tenure_table[i] = new long long int[num_color];
        long long int* tabu_tenure_table_i = tabu_tenure_table[i];
        for (int j = 0; j < num_color; j++)
        {
            tabu_tenure_table_i[j] = 0;
        }
    }

    moved = {-1, -1};
    equal_nontabu_delta.resize(2000, {-1, -1});
    equal_tabu_delta.resize(2000, {0,0});
    min_delta = 999999;
    iter = 0;

    p1_conflict = 0;
    p2_conflict = 0;
    elite1_conflict = 0;
    elite2_conflict = 0;
    best_solution_conflict = 0;
    final_solution.resize(num_vertex, 0);
    for (int i = 0; i < num_vertex; i++)
        final_solution[i] = pseudoRandNumGen() % num_color;

    // add edge information from dataset to class;
    for (int i = 0;i < input_edge_num; i++)
    {
        int v1 = input_edges[i][0];
        int v2 = input_edges[i][1];

        adj_list[v1][vertex_edge_num[v1]] = v2;
        vertex_edge_num[v1]++;

        adj_list[v2][vertex_edge_num[v2]] = v1;
        vertex_edge_num[v2]++;
    }

    // debug variables;
    max_equal_nontabu_count = 0;
    max_equal_tabu_count = 0;
    start_time = clock();
    end_time = 0;
}


Hybrid_Evolution::~Hybrid_Evolution()
{
    for (int i = 0; i < num_vertex; i++)
    {
        delete[] adj_list[i];
        delete[] adj_color_table[i];
        delete[] tabu_tenure_table[i];
    }
    delete[] adj_list;
    delete[] adj_color_table;
    delete[] tabu_tenure_table;
}


void Hybrid_Evolution::insert_adj_list(int v1, int v2)
{
    adj_list[v1][vertex_edge_num[v1]] = v2;
    vertex_edge_num[v1]++;

    adj_list[v2][vertex_edge_num[v2]] = v1;
    vertex_edge_num[v2]++;
}


void Hybrid_Evolution::find_move(vector<unsigned int> &solution)
{
    min_delta = 999999;
    int tabu_delta = 999999;
    int equal_nontabu_count = 0;
    int equal_tabu_count = 0;

    int aspiration = best_history_conflict - conflict;

    for (int i = 0; i < num_vertex; i++) // i is vertex;
    {
        unsigned int solution_i = solution[i]; // solution_i is color;
        int* adj_color_table_i = adj_color_table[i];

        if (adj_color_table_i[solution_i] > 0) // if vertex i overlap its neighbor's color;
        {
            long long int* tabu_tenure_table_i = tabu_tenure_table[i];
            for (int j = 0; j < num_color; j++) // j is color;
            {
                if (solution_i != j) // find a new color;
                {//cpu流水线
                    int this_delta = adj_color_table_i[j] - adj_color_table_i[solution_i]; // new-old, the less the better;
                    if (tabu_tenure_table_i[j] <= iter) //nontabu move;
                    {
                        if (this_delta < min_delta)
                        {//分支预判惩罚 6.0
                            equal_nontabu_count = 0;
                            min_delta = this_delta;

                            equal_nontabu_delta[equal_nontabu_count].u = i; // i is vertex;
                            equal_nontabu_delta[equal_nontabu_count].vj = j; // j is color;

                            equal_nontabu_count++;
                        }
                        else if (this_delta == min_delta)
                        {
                            equal_nontabu_delta[equal_nontabu_count].u = i; // i is vertex;
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

                            equal_tabu_delta[equal_tabu_count].u = i; // i is vertex;
                            equal_tabu_delta[equal_tabu_count].vj = j; // j is color;

                            equal_tabu_count++;
                        }
                        else if(this_delta == tabu_delta)
                        {
                            equal_tabu_delta[equal_tabu_count].u = i; // i is vertex;
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

    if (conflict < best_history_conflict)
        best_history_conflict = conflict; // update minimum conflict of history;

    unsigned int old_color = solution[moved.u];
    solution[moved.u] = moved.vj;
    // 用这种方法只需要更新iter就可以了, 而不需要更新整个tabu_tenure_table; 因为tabu_tenure+old_iter-new_iter等价于tabu_tenure--
    // in each iteration;
    tabu_tenure_table[moved.u][old_color] = iter + conflict + pseudoRandNumGen() % 10 + 1;

    // update adjacent color table;
    for (int i = 0; i < vertex_edge_num[moved.u]; i++)
    {
        int adj_list_node_moved_i = adj_list[moved.u][i];

        adj_color_table[adj_list_node_moved_i][old_color]--;
        adj_color_table[adj_list_node_moved_i][moved.vj]++;
    }
}


void Hybrid_Evolution::tabu_search(vector<unsigned int> &solution, bool is_limit, long long int max_iter)
{
    // compute initial conflict;
    for (int i = 0; i < num_vertex; i++)
    {
        int num_edge = vertex_edge_num[i];
        unsigned int this_vertex_color = solution[i];
        int* adj_color_table_i = adj_color_table[i];

        int* adj_list_i = adj_list[i];
        for (int j = 0; j < num_edge; j++)
        {
            unsigned int adj_color = solution[adj_list_i[j]];

            if (this_vertex_color == adj_color)
                conflict++;

            adj_color_table_i[adj_color]++; // initialize adjacent color table;
        }
    }

    conflict = conflict / 2;
    best_history_conflict = conflict;
    // cerr << "initial number of confilcts:" << conflict << endl;

    iter = 0;

    if(is_limit) // True: set upper bound of iteration, used in HEA;
    {
        while (iter < max_iter && conflict > 0)
        {
            iter++;

            find_move(solution);
            make_move(solution);
        }
    }
    else // False: used in tabu search;
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


void Hybrid_Evolution::cross_over(const Solution_Partition& s1, const Solution_Partition& s2, vector<unsigned int>& child) const
{
    int A, B;
    Solution_Partition s[2] = {Solution_Partition(num_vertex, num_color), Solution_Partition(num_vertex, num_color)};
    s[0] = s1;
    s[1] = s2;

    for (int i = 0; i < num_color; i++) // 遍历所有颜色;
    {
        // 如果循环轮次是奇数，摘取s1的独立集；如果循环数是偶数，摘取s2的独立集。
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
            child[point] = i; // 将解中的对应顶点point{2,5,6,7,10}改成颜色i; 为啥不是max_index? 因为伪代码就是这样;

            //j循环的过程中, 在B中删除这些点{2,5,6,7,10}
            unsigned int color = s[B].solution[point]; // 找出顶点{2,5,6,7,10}在B中的颜色;
            unsigned int partition_index = s[B].partition_index[point]; // 找出顶点{2,5,6,7,10}在分划B中的位置;

            // --s[B].color_num[color]; // 每删除顶点{2,5,6,7,10}中的一个, 就把顶点{2,5,6,7,10}在B中的颜色数量-1;
            s[B].partition[color][partition_index] = s[B].partition[color][--s[B].num_colors[color]]; // 把一个分划中末尾的顶点填补到删除顶点的位置;
            int t = s[B].partition[color][partition_index]; // 一个颜色分划中原先位于末尾, 现在填补到被删除顶点的顶点名字;
            s[B].partition_index[t] = partition_index; // 将替换到被删除顶点的顶点在分划中的位置更新为被删除顶点的位置;
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
            child[point] = color; // 给颜色i独立集中第j个顶点分配一种随机的颜色;
        }
    }
}


void Hybrid_Evolution::hybrid_evolution_duet_1(long long int max_iter)
{
    // Line 1: p1, p2, elite1, elite2, best <- init()
    for (int i = 0; i < num_vertex; i++)
    {
        p1.solution[i] = pseudoRandNumGen() % num_color;
    }

    for (int i = 0; i < num_vertex; i++)
    {
        p2.solution[i] = pseudoRandNumGen() % num_color;
    }

    // construct partition for each solution in the solution;
    p1.construct_partition();
    p2.construct_partition();

    for (int i = 0; i < num_vertex; i++)
    {
        elite1.solution[i] = pseudoRandNumGen() % num_color;
    }

    for (int i = 0; i < num_vertex; i++)
    {
        elite2.solution[i] = pseudoRandNumGen() % num_color;
    }

    // evaluate LINE 1
    // cerr << "p1: ";
    // print_array(population_solution[0].solution);
    // cerr << "p2: ";
    // print_array(population_solution[1].solution);
    // cerr << "elite1: ";
    // print_array(elite1.solution);
    // cerr << "elite2: ";
    // print_array(elite2.solution);

    // cerr << "best solution: ";
    // print_array(best_solution.solution);

    // Line 1: p1, p2, elite1, elite2, best <- init()
    for (int i = 0; i < num_vertex; i++)
        best_solution.solution[i] = pseudoRandNumGen() % num_color;
    best_solution_conflict = compute_conflict(best_solution.solution);

    Solution_Partition c1(num_vertex, num_color);
    Solution_Partition c2(num_vertex, num_color);

    // Line 2: generation, cycle <- 0
    long long int generation = 0;
    int cycle = 0;
    // Line 3 and Line 10: do while
    while (best_solution_conflict > 0 && p1.solution != p2.solution && generation < 1)
    {
        // random select two index from population as parents;

        // 因为num_colors定义着边界, 所以只有num_colors需要置零;
        memset(&c1.num_colors[0], 0, sizeof(c1.num_colors[0]) * c1.num_colors.size());
        memset(&c2.num_colors[0], 0, sizeof(c2.num_colors[0]) * c2.num_colors.size());
        // cerr << "After 2: " << child.color_num[17] << endl; // debug memset sentence;

        // debug information
        // cerr << "p1 before cross over: " << endl;
        // test.population_solution[p1].print_population_solution();
        // cerr << "p2 before cross over: " << endl;
        // test.population_solution[p2].print_population_solution();

        // Line 4: c1 <- GPX(p1, p2)
        cross_over(p1, p2, c1.solution);
        // c1.construct_partition();
        // cerr << "c1 structure: " << endl;
        // c1.print_solution_partition();
        // LINE 5: c2 <- GPX(p2, p1)
        cross_over(p2, p1, c2.solution);
        // c2.construct_partition();
        // cerr << "c2 structure: " << endl;
        // c2.print_solution_partition();

        // LINE 6: p1 <- TabuCol(c1,IterTC)
        // reset adj_color_table and tabu_tenure_table to zero;
        // adj_list不需要重置, 因为图的结构(点和边的关系)是不变的, 变的只是颜色;
        for (int i = 0; i < num_vertex; i++)
        {
            memset(adj_color_table[i], 0, num_color * sizeof(int));
            memset(tabu_tenure_table[i], 0, num_color * sizeof(long long int));
        }

        conflict = 0;
        best_history_conflict = 0;

        tabu_search(c1.solution, true, max_iter);
        c1.construct_partition();
        // cerr << "conflict of p1 before tabu: ";
        // cerr << compute_conflict(population_solution[0].solution) << endl;
        p1 = c1;
        p1_conflict = conflict;
        // cerr << "conflict of p1 after tabu: ";
        // cerr << compute_conflict(population_solution[0].solution) << endl;

        // LINE 7: p1 <- TabuCol(c2,IterTC)
        // reset adj_color_table and tabu_tenure_table to zero;
        // adj_list不需要重置, 因为图的结构(点和边的关系)是不变的, 变的只是颜色;
        for (int i = 0; i < num_vertex; i++)
        {
            memset(adj_color_table[i], 0, num_color * sizeof(int));
            memset(tabu_tenure_table[i], 0, num_color * sizeof(long long int));
        }

        conflict = 0;
        best_history_conflict = 0;

        tabu_search(c2.solution, true, max_iter);
        c2.construct_partition();
        // cerr << "conflict of p2 before tabu: ";
        // cerr << compute_conflict(population_solution[1].solution) << endl;
        p2 = c2;
        p2_conflict = conflict;
        // cerr << "conflict of p2 after tabu: ";
        // cerr << compute_conflict(population_solution[1].solution) << endl;

        // LINE 8: best <- saveBest(p1, p2, best)
        // 找出种群中的最大冲突数;
        if(p1_conflict < p2_conflict) // p1 < p2;
        {
            if(p1_conflict < best_solution_conflict)
            {
                best_solution_conflict = p1_conflict;
                best_solution = p1;
            }
        }
        else // p1 >= p2;
        {
            if(p2_conflict < best_solution_conflict)
            {
                best_solution_conflict = p2_conflict;
                best_solution = p2;
            }
        }

        // cerr << population_num_conflict[0] << endl;
        // cerr << population_num_conflict[1] << endl;
        // cerr << best_solution_conflict << endl;

        ///*
        if(generation % 100 == 0)
        {
            double elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
            cerr << "Generation: " << generation <<"  ";
            cerr << "elapsed time(s): " << elapsed_time << endl;
            cerr << "best solution conflict: " << best_solution_conflict << endl;
        }
        //*/

        generation++;
    }

    end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    cerr << "Generation: " << generation << " ";
    cerr << "elapsed time(s): " << elapsed_time << " ";
    cerr << "Generation frequency: " << double (generation) / elapsed_time << endl;

    if (best_solution_conflict == 0)
    {
        cerr << "color of each vertex: ";
        for(int i=0;i<num_vertex;i++)
        {
            cerr << best_solution.solution[i] << " ";
        }
        cerr << endl;

        cerr << "conflict of best solution: ";
        cerr << compute_conflict(best_solution.solution) << endl;
    }
    else
        cerr << "over time" << endl;
}


// debug function: compute conflict of a 解
int Hybrid_Evolution::compute_conflict(vector<unsigned int> &solution)
{
    int this_conflict = 0;
    //cerr <<endl;

    for (int i = 0; i < num_vertex; i++)
    {
        int num_edge = vertex_edge_num[i];
        unsigned int this_vertex_color = solution[i];

        int* adj_list_i = adj_list[i];
        for (int j = 0; j < num_edge; j++)
        {
            unsigned int adj_color = solution[adj_list_i[j]];

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
        int* adj_list_i = adj_list[i];
        for (int j = 0;j < vertex_edge_num[i];j++)
        {
            cerr << adj_list_i[j] << " ";
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

// debug function:
void Hybrid_Evolution::print_array(vector<unsigned int> &solution)
{
    for(unsigned int i : solution)
    {
        cerr << i << " ";
    }
    cerr << endl;
}


/*
 * debugging command:
 * laptop: C:\wamp64\www\npbenchmark\npbenchmark-main\GCP_HEA_dynamic
 * home server: /home/ssfc/GCP_HEA_dynamic
 * g++ hea.cpp -g; gdb a.out
 * r 11 6 chvatal.txt
 * r 11 6 DSJC0250.9.txt
 * r 11 6 DSJC1000.9.txt
 */