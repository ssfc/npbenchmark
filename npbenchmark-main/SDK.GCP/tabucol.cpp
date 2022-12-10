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
void initRand(int seed) { pseudoRandNumGen = mt19937(seed); }

// constructor;
Graph::Graph(int input_num_vertex, int input_edge_num, int input_num_color, vector<array<int, 2>>& input_edges, int seed)
{
    num_vertex = input_num_vertex;
    num_color = input_num_color;
    initRand(seed);

    conflict = 0;

    try
    {
        // allocate memory to adjacent list (num_vertex * num_vertex) and vertex edge (num_vertex);
        adj_list = new int* [num_vertex];
        vertex_edge_num = new int[num_vertex];

        for (int i = 0; i < num_vertex; i++)
        {
            adj_list[i] = new int[num_vertex];
            vertex_edge_num[i] = 0;
        }

        // the reason to use adj_list_i is to accelerate;
        for (int i = 0; i < num_vertex; i++)
        {
            int* adj_list_i = adj_list[i];
            for (int j = 0; j < num_vertex; j++)
            {
                adj_list_i[j] = 0;
            }
        }

        // allocate memory and initial value for solution (dim, num_vertex);
        solution = new unsigned int[num_vertex];
        for (int i = 0; i < num_vertex; i++)
            solution[i] = pseudoRandNumGen() % num_color;//初始化颜色

        // allocate memory to conflict color table (num_vertex * num_color) and tenure vertex color (num_vertex * num_color);
        adj_color_table = new int* [num_vertex];
        tabu_tenure_table = new unsigned int* [num_vertex];

        for (int i = 0; i < num_vertex; i++)
        {
            adj_color_table[i] = new int[num_color];
            tabu_tenure_table[i] = new unsigned int[num_color];
        }

        // allocate initial value to conflict color table (num_vertex * num_color) and tenure vertex color (num_vertex * num_color);
        for (int i = 0; i < num_vertex; i++)
        {
            int* adj_color_table_i = adj_color_table[i];
            unsigned int* tabu_tenure_table_i = tabu_tenure_table[i];

            for (int j = 0; j < num_color; j++)
            {
                adj_color_table_i[j] = 0;
                tabu_tenure_table_i[j] = 0;
            }
        }

        // allocate memory to equ_delta;
        equal_nontabu_delta = new int* [2000];
        for(int i=0;i<2000;i++)
        {
            equal_nontabu_delta[i] = new int[2];
        }

        // allocate memory to equ_tabu_delta;
        equal_tabu_delta = new int* [2000];
        for(int i=0;i<2000;i++)
        {
            equal_tabu_delta[i] = new int[2];
        }

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
    for (int i = 0; i < num_vertex; i++)
    {
        delete[] tabu_tenure_table[i];
        delete[] adj_color_table[i];
        delete[] adj_list[i];
    }
    delete[] solution;
    delete[] tabu_tenure_table;
    delete[] adj_color_table;
    delete[] adj_list;
}

// class: print adjacency list of graph;
void Graph::print_graph() const
{
    cerr << "Adjacency list of graph: " << num_vertex << " " << num_color << endl;
    for (int i = 0; i < num_vertex; i++)
    {
        int* adj_list_i = adj_list[i];
        for (int j = 0;j < vertex_edge_num[i];j++)
        {
            cerr << adj_list_i[j] << " ";
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

    int A = best_conflict - conflict;

    for (int i = 0; i < num_vertex; i++) // i is vertex;
    {
        unsigned int solution_i = solution[i]; // solution_i is color;

        // use one-dimensional array to save addressing time;
        int* adj_color_table_i = adj_color_table[i];
        unsigned int* tabu_tenure_table_i = tabu_tenure_table[i];

        if (adj_color_table_i[solution_i] > 0) // if vertex i overlap its neighbor's color;
        {
            for (int j = 0; j < num_color; j++) // j is color;
            {
                if (solution_i != j) // find a new color;
                {//cpu流水线
                    int tmp = adj_color_table_i[j] - adj_color_table_i[solution_i]; // new-old, the less the better;
                    if (tabu_tenure_table_i[j] <= iter) //非禁忌移动;
                    {
                        if (tmp < min_delta)
                        {//分支预判惩罚 6.0
                            equal_nontabu_count = 0;
                            min_delta = tmp;

                            equal_nontabu_delta[equal_nontabu_count][0] = i; // i is vertex;
                            equal_nontabu_delta[equal_nontabu_count][1] = j; // j is color;

                            equal_nontabu_count++;
                        }
                        else if (tmp == min_delta)
                        {
                            equal_nontabu_delta[equal_nontabu_count][0] = i; // i is vertex;
                            equal_nontabu_delta[equal_nontabu_count][1] = j; // j is color;

                            equal_nontabu_count++;
                        }
                    }
                    else // 禁忌移动;
                    {
                        if (tmp < tabu_delta)
                        {//6.0
                            tabu_delta = tmp;
                            equal_tabu_count = 0;

                            equal_tabu_delta[equal_tabu_count][0] = i; // i is vertex;
                            equal_tabu_delta[equal_tabu_count][1] = j; // j is color;

                            equal_tabu_count++;
                        }
                        else if(tmp == tabu_delta)
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

    if (tabu_delta < A && tabu_delta < min_delta)
    {
        min_delta = tabu_delta;
        unsigned int rand_select = pseudoRandNumGen() % equal_tabu_count; // 相等tabu_delta随机选择
        // cerr << "random select tabu: " << rand_select << endl;
        node_moved = equal_tabu_delta[rand_select][0];
        color_moved = equal_tabu_delta[rand_select][1];
    }
    else
    {
        unsigned int rand_select = pseudoRandNumGen() % equal_nontabu_count; // 相等delta随机选择
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

    unsigned int old_color = solution[node_moved];
    solution[node_moved] = color_moved;
    tabu_tenure_table[node_moved][old_color] = iter + conflict + pseudoRandNumGen() % 10 + 1; //更新禁忌表

    int* adj_list_node_moved = adj_list[node_moved];

    // update adjacent color table;
    for (int i = 0; i < vertex_edge_num[node_moved]; i++)
    {
        int adj_list_node_moved_i = adj_list_node_moved[i];

        int* adj_color_table_adj_list_node_moved_i = adj_color_table[adj_list_node_moved_i];

        adj_color_table_adj_list_node_moved_i[old_color]--;
        adj_color_table_adj_list_node_moved_i[color_moved]++;
    }
}

// class: tabu search;
unsigned int Graph::tabu_search()
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
unsigned int Graph::get_solution(int i)
{
    return solution[i];
}

