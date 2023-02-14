//
// Created by take_ on 2023/1/25.
//

#ifndef HEAD1_H
#define HEAD1_H

#include <algorithm>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iostream>
#include <fstream>
#include <random>
#include <vector>

using namespace std;


struct Move {
    int u;
    int vj;
};

// 整个类可以用一个sol构造出来;
class Solution_Partition {
public:
    unsigned int num_vertex;
    // dimension, num_vertex;
    // element: amount of color i the sol;
    // meaning: sol;
    vector<unsigned int> solution;
    // dimension: num_color * num_vertex;
    // element: vertex name (i);
    // meaning: 将属于某颜色的独立集所有成员顶点按顺序排列, 范围之外的置零;
    vector<vector<int>> partition;
    // dimension: num_vertex;
    // element: 序号;
    // meaning: 顶点i在所属颜色独立集中的序号; 另外一种解读是, 该颜色在该sol中是第几次出现;
    vector<unsigned int> partition_index;
    // dimension, num_color;
    // element: num of color i in the sol;
    // meaning: 储存一个解中每种颜色的顶点数量, 目的是cross_over中计算最大独立集;
    vector<int> num_colors;

    explicit Solution_Partition(int input_num_vertex, int input_num_color);
    void construct_partition();
    ~Solution_Partition();

    // debug function
    void print_solution_partition();
};


// class HE;
class Hybrid_Evolution
{
private:
    long long int iter;

public:
    // variables;
    int num_vertex;
    int num_color;
    int** adj_list; // adjacency list; dimension, num_vertex * num_vertex;
    vector<int> vertex_edge_num; // number of edge of each vertex; dimension, num_vertex;

    int conflict;
    int best_history_conflict;

    int** adj_color_table; // dimension,
    long long int** tabu_tenure_table;

    Move moved;
    vector<Move> equal_nontabu_delta; //非禁忌相同delta值
    vector<Move> equal_tabu_delta; //禁忌相同delta值
    int min_delta; // 移动增量

    int num_population;
    vector<Solution_Partition> population_solution;
    int population_min_conflict; // the min conflict among the population collection;
    int population_min_conflict_index;
    vector<int> population_num_conflict;
    Solution_Partition best_solution;
    vector<unsigned int> final_solution;

    // debug variables:
    int max_equal_nontabu_count;
    int max_equal_tabu_count;
    double start_time;
    double end_time;

    // functions
    Hybrid_Evolution(int input_num_vertex, int input_edge_num, int input_num_color, vector<array<int, 2>>& input_edges, int input_num_population, int input_seed);
    ~Hybrid_Evolution();

    void insert_adj_list(int i, int j);

    void find_move(vector<unsigned int> &solution);
    void make_move(vector<unsigned int> &solution);
    void tabu_search(vector<unsigned int> &solution, bool is_limit, long long int max_iter);

    void cross_over(const Solution_Partition& s1, const Solution_Partition& s2, vector<unsigned int> &child) const;
    void hybrid_evolution_duet_1(long long int max_iter);

    // debug function: compute conflict of a solution
    int compute_conflict(vector<unsigned int> &solution);
    [[nodiscard]] long long int get_iteration() const;
    void print_adj_list() const; // print adjacent list of graph;
    static void print_array(vector<unsigned int> &solution);

    [[nodiscard]] int get_max_equal_nontabu_count() const;
    [[nodiscard]] int get_max_equal_tabu_count() const;
};


#endif //HEAD1_H