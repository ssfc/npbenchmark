//
// Created by take_ on 2022/11/3.
//
#include "tabucol.h"
#include <climits>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

// random number generator.
mt19937 pseudoRandNumGen;
void initRand(int seed) { pseudoRandNumGen = mt19937(seed); }

// constructor;
Graph::Graph(int input_num_vertex, int input_num_color)
{
    num_vertex = input_num_vertex;
    num_color = input_num_color;

    try
    {
        adj_list = new int* [num_vertex];
        vertex_edge = new int[num_vertex];

        for (int i = 0; i < num_vertex; i++)
        {
            adj_list[i] = new int[num_vertex];
            vertex_edge[i] = 0;
        }

        for (int i = 0; i < num_vertex; i++)
        {
            int* adj_list_i = adj_list[i];
            for (int j = 0; j < num_vertex; j++)
            {
                adj_list_i[j] = 0;
            }
        }
    }
    catch (const bad_alloc& e)
    {
        cerr << "图内存分配失败" << e.what() << endl;
    }
}

// class: allocate memory;
void Graph::allocate_memory()
{
    try
    {
        solution = new int[num_vertex];
        adj_color_table = new int* [num_vertex];
        tabu_tenure_table = new int* [num_vertex];

        for (int i = 0; i < num_vertex; i++)
        {
            adj_color_table[i] = new int[num_color];
            tabu_tenure_table[i] = new int[num_color];
        }

        for (int i = 0; i < num_vertex; i++)
        {
            int* adj_color_table_i = adj_color_table[i];
            int* tabu_tenure_table_i = tabu_tenure_table[i];

            for (int j = 0; j < num_color; j++)
            {
                adj_color_table_i[j] = 0;
                tabu_tenure_table_i[j] = 0;
            }
        }
    }
    catch (const bad_alloc& e)
    {
        cerr << "初始化内存分配失败:" << e.what() << endl;
    }
}

// free the memory;
void Graph::free_memory() const
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

//初始化，分组顶点颜色，计算初始冲突值，初始化邻接颜色表
void Graph::initialization(int seed)
{
    conflict = 0;
    allocate_memory(); //初始化内存分配

    // srand(seed);
    initRand(seed);
    for (int i = 0; i < num_vertex; i++)
        solution[i] = pseudoRandNumGen() % num_color;//初始化颜色

    /*
    cerr << "initial solution: ";
    for (int i = 0; i < num_vertex; i++)
        cerr << solution[i] << " ";
    cerr << endl;
    */

    for (int i = 0; i < num_vertex; i++)
    {
        int num_edge = vertex_edge[i];
        int this_vertex_color = solution[i];

        int* adj_color_table_i = adj_color_table[i];
        int* adj_list_i = adj_list[i];

        for (int j = 0; j < num_edge; j++)
        {
            int adj_color = solution[adj_list_i[j]];

            if (this_vertex_color == adj_color)
                conflict++;

            adj_color_table_i[adj_color]++; // initialize adjacent color table;
        }
    }

    conflict = conflict / 2;
    best_conflict = conflict;
    cerr << "init number of confilcts:" << conflict << endl;
}

// class: print adjacency list of graph;
void Graph::print_graph() const
{
    cerr << "Adjacency list of graph: " << num_vertex << " " << num_color << endl;
    for (int i = 0; i < num_vertex; i++)
    {
        int* adj_list_i = adj_list[i];
        for (int j = 0;j < vertex_edge[i];j++)
        {
            cerr << adj_list_i[j] << " ";
        }
        cerr << endl;
    }
}

// class: find best move (tabu or not tabu);
void Graph::find_move()
{
    delta = INT_MAX;
    int tabu_delta = INT_MAX;
    int count = 0;
    int tabu_count = 0;

    int A = best_conflict - conflict;

    for (int i = 0; i < num_vertex; i++) // i is vertex;
    {
        int solution_i = solution[i]; // solution_i is color;

        // use one-dimensional array to save addressing time;
        int* adj_color_table_i = adj_color_table[i];
        int* tabu_tenure_table_i = tabu_tenure_table[i];

        if (adj_color_table_i[solution_i] > 0) // if vertex i overlap its neighbor's color;
        {
            for (int j = 0; j < num_color; j++) // j is color;
            {
                if (solution_i != j) // find a new color;
                {//cpu流水线
                    //非禁忌移动
                    int tmp = adj_color_table_i[j] - adj_color_table_i[solution_i];
                    if (tabu_tenure_table_i[j] <= iter)
                    {
                        if (tmp <= delta)
                        {//分支预判惩罚 6.0
                            if (tmp < delta)
                            {
                                count = 0;
                                delta = tmp;
                            }

                            equ_delta[count][0] = i;
                            equ_delta[count][1] = j;

                            count++;
                        }
                    }
                    else
                    {//禁忌移动
                        if (tmp <= tabu_delta)
                        {//6.0
                            if (tmp < tabu_delta)
                            {
                                tabu_delta = tmp;
                                tabu_count = 0;
                            }

                            equ_tabu_delta[tabu_count][0] = i;
                            equ_tabu_delta[tabu_count][1] = j;

                            tabu_count++;
                        }
                    }
                }
            }
        }
    }

    if (tabu_delta < A && tabu_delta < delta)
    {
        delta = tabu_delta;
        int rand_select = pseudoRandNumGen() % tabu_count; // 相等tabu_delta随机选择
        node_moved = equ_tabu_delta[rand_select][0];
        color_moved = equ_tabu_delta[rand_select][1];
    }
    else
    {
        int rand_select = pseudoRandNumGen() % count; // 相等delta随机选择
        node_moved = equ_delta[rand_select][0];
        color_moved = equ_delta[rand_select][1];
    }
}

// class: 更新值
void Graph::make_move()
{
    conflict = delta + conflict; // update value of conflict;

    if (conflict < best_conflict)
        best_conflict = conflict; // update minimum conflict of history;

    int old_color = solution[node_moved];
    solution[node_moved] = color_moved;
    tabu_tenure_table[node_moved][old_color] = iter + conflict + pseudoRandNumGen() % 10 + 1; //更新禁忌表

    int* adj_list_node_moved = adj_list[node_moved];

    // update adjacent color table;
    for (int i = 0; i < vertex_edge[node_moved]; i++)
    {
        int adj_list_node_moved_i = adj_list_node_moved[i];

        int* adj_color_table_adj_list_node_moved_i = adj_color_table[adj_list_node_moved_i];

        adj_color_table_adj_list_node_moved_i[old_color]--;
        adj_color_table_adj_list_node_moved_i[color_moved]++;
    }
}

// class: tabu search;
void Graph::tabu_search()
{
    ofstream ofile("total_O3.txt", ios::out);
    double start_time, end_time;
    double elapsed_time;

    start_time = clock();
    iter = 0;
    while (conflict > 0)
    {
        iter++;
        // cerr << "iter: " << iter << endl;
        find_move();
        make_move();
    }

    end_time = clock();
    elapsed_time = (double(end_time - start_time)) / CLOCKS_PER_SEC;

    cerr << "success, iterations: " << iter << " elapsed_time(s): " << elapsed_time << " frequency:" << double(iter / elapsed_time) << endl;

    // save solutions;
    for (int i = 0;i < num_vertex; i++)
    {
        ofile << solution[i] << endl;
    }

    ofile.close();
}