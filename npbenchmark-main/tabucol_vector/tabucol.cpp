//
// Created by take_ on 2022/11/3.
//
#include "tabucol.h"
#include <climits>
#include <fstream>
#include <iostream>
#include <random>

using namespace std;

// random number generator.
mt19937 pseudoRandNumGen;
void init_rand(int seed) { pseudoRandNumGen = mt19937(seed); }

// constructor;
Graph::Graph(int input_num_vertex, int input_edge_num, int input_num_color, vector<array<int, 2>>& input_edges, int seed)
{
    num_vertex = input_num_vertex;
    num_color = input_num_color;
    init_rand(seed);

    conflict = 0;

    try
    {
        // allocate memory to adjacent list (num_vertex * num_vertex) and vertex edge (num_vertex);
        adj_list.resize(num_vertex);
        vertex_edge_num.resize(num_vertex);

        for (int i = 0; i < num_vertex; i++)
        {
            adj_list[i].resize(num_vertex);
            vertex_edge_num[i] = 0;
        }

        // allocate memory and initial value for solution (dim, num_vertex);
        solution.resize(num_vertex);
        for (int i = 0; i < num_vertex; i++)
            solution[i] = pseudoRandNumGen() % num_color;//初始化颜色

        // allocate memory to conflict color table (num_vertex * num_color) and tenure vertex color (num_vertex * num_color);
        adj_color_table.resize(num_vertex);
        tabu_tenure_table.resize(num_vertex);

        for (int i = 0; i < num_vertex; i++)
        {
            adj_color_table[i].resize(num_color);
            tabu_tenure_table[i].resize(num_color);
        }

        // allocate memory to equ_delta;
        equal_nontabu_delta.resize(2000);
        for(int i=0;i<2000;i++)
        {
            equal_nontabu_delta[i].resize(2);
        }

        // allocate memory to equ_tabu_delta;
        equal_tabu_delta.resize(2000);
        for(int i=0;i<2000;i++)
        {
            equal_tabu_delta[i].resize(2);
        }

        for (int i = 0; i < num_vertex; i++)
        {
            int num_edge = vertex_edge_num[i];
            int this_vertex_color = solution[i];

            for (int j = 0; j < num_edge; j++)
            {
                int adj_color = solution[adj_list[i][j]];

                if (this_vertex_color == adj_color)
                    conflict++;

                adj_color_table[i][adj_color]++; // initialize adjacent color table;
            }
        }

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

        // compute initial conflict;
        for (int i = 0; i < num_vertex; i++)
        {
            int num_edge = vertex_edge_num[i];
            int this_vertex_color = solution[i];

            for (int j = 0; j < num_edge; j++)
            {
                int adj_color = solution[adj_list[i][j]];

                if (this_vertex_color == adj_color)
                    conflict++;

                adj_color_table[i][adj_color]++; // initialize adjacent color table;
            }
        }

        conflict = conflict / 2;
        best_conflict = conflict;
        cerr << "initial number of confilcts:" << conflict << endl;

    }
    catch (const bad_alloc& e)
    {
        cerr << "初始化内存分配失败:" << endl;
    }
}

// free the memory;
Graph::~Graph()
{

}

// class: print adjacency list of graph;
void Graph::print_graph() const
{
    cerr << "Adjacency list of graph: " << num_vertex << " " << num_color << endl;
    for (int i = 0; i < num_vertex; i++)
    {
        for (int j = 0;j < vertex_edge_num[i];j++)
        {
            cerr << adj_list[i][j] << " ";
        }
        cerr << endl;
    }
}

// class: find best move (tabu or not tabu);
void Graph::find_move()
{
    min_delta = INT_MAX;
    int tabu_delta = INT_MAX;
    int equal_nontabu_count = 0;
    int equal_tabu_count = 0;

    int aspiration = best_conflict - conflict;

    for (int i = 0; i < num_vertex; i++) // i is vertex;
    {
        int solution_i = solution[i]; // solution_i is color;

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

                            equal_nontabu_delta[equal_nontabu_count][0] = i; // i is vertex;
                            equal_nontabu_delta[equal_nontabu_count][1] = j; // j is color;

                            equal_nontabu_count++;
                        }
                        else if (this_delta == min_delta)
                        {
                            equal_nontabu_delta[equal_nontabu_count][0] = i; // i is vertex;
                            equal_nontabu_delta[equal_nontabu_count][1] = j; // j is color;

                            equal_nontabu_count++;
                        }
                    }
                    else // tabu move;
                    {
                        if (this_delta < tabu_delta)
                        {//6.0
                            equal_tabu_count = 0;
                            tabu_delta = this_delta;

                            equal_tabu_delta[equal_tabu_count][0] = i; // i is vertex;
                            equal_tabu_delta[equal_tabu_count][1] = j; // j is color;

                            equal_tabu_count++;
                        }
                        else if(this_delta == tabu_delta)
                        {
                            equal_tabu_delta[equal_tabu_count][0] = i; // i is vertex;
                            equal_tabu_delta[equal_tabu_count][1] = j; // j is color;

                            equal_tabu_count++;
                        }

                    }
                }
            }
        }
    }

    if (tabu_delta < aspiration && tabu_delta < min_delta)
    {
        min_delta = tabu_delta;
        int rand_select = pseudoRandNumGen() % equal_tabu_count; // 相等tabu_delta随机选择
        // cerr << "random select tabu: " << rand_select << endl;
        node_moved = equal_tabu_delta[rand_select][0];
        color_moved = equal_tabu_delta[rand_select][1];
    }
    else
    {
        int rand_select = pseudoRandNumGen() % equal_nontabu_count; // 相等delta随机选择
        // cerr << "random select nontabu: " << rand_select << endl;
        node_moved = equal_nontabu_delta[rand_select][0];
        color_moved = equal_nontabu_delta[rand_select][1];
    }
}

// class: 更新值
void Graph::make_move()
{
    conflict = min_delta + conflict; // update value of conflict;

    if (conflict < best_conflict)
        best_conflict = conflict; // update minimum conflict of history;

    int old_color = solution[node_moved];
    solution[node_moved] = color_moved;
    tabu_tenure_table[node_moved][old_color] = iter + conflict + pseudoRandNumGen() % 10 + 1; //更新禁忌表

    // update adjacent color table;
    for (int i = 0; i < vertex_edge_num[node_moved]; i++)
    {
        int adj_list_node_moved_i = adj_list[node_moved][i];

        adj_color_table[adj_list_node_moved_i][old_color]--;
        adj_color_table[adj_list_node_moved_i][color_moved]++;
    }
}

// class: tabu search;
int Graph::tabu_search()
{
    iter = 0;
    while (conflict > 0)
    {
        iter++;
        // cerr << "iter: " << iter << endl;
        find_move();
        make_move();
    }

    return iter;
}

// get solution;
int Graph::get_solution(int i)
{
    return solution[i];
}

// running command:
// g++ main.cpp GraphColoring.cpp tabucol.cpp -O3; ./a.out 999999 6 <./data/DSJC0250.9.txt >sln.0250.9.txt

// debugging command:
// In shell:
// g++ main.cpp GraphColoring.cpp tabucol.cpp -O3 -g; gdb a.out
// In GDB:
// r 999999 6 <./data/DSJC0250.9.txt >sln.0250.9.txt