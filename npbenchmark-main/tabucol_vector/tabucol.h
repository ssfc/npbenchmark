//
// Created by take_ on 2022/11/3.
//

#ifndef SDK_GCP_TABUCOL_H
#define SDK_GCP_TABUCOL_H

#include <array>
#include <ctime>
#include <vector>

using namespace std;

class Graph
{
private:
    int num_vertex; // number of vertex in the graph;
    int num_color; // number of color in the graph;

    vector<vector<int>> adj_list; // adjacency list; dimension, num_vertex * num_vertex;
    // each row is 11...1000, the number of 1 is the number of vertex adjacent to it, recording in vertex_edge;

    vector<int> vertex_edge_num; // number of edge of each vertex; dimension, num_vertex;

    vector<int> solution; // color of each vertex; dimension, num_vertex;
    int conflict;
    int best_conflict; // 历史最好的冲突值

    vector<vector<long long int>> tabu_tenure_table; // tenure of each color on each node; dimension, num_vertex * num_color;
    vector<vector<int>> adj_color_table; // conflicts of each color on each node; dimension, num_vertex * num_color;

    int min_delta; // 移动增量
    int node_moved; // 每次移动的结点
    int color_moved; //每次移动的颜色
    long long int iter;

    vector<vector<int>> equal_nontabu_delta; //非禁忌相同delta值
    vector<vector<int>> equal_tabu_delta; //禁忌相同delta值

    // debug variables;
    int max_equal_nontabu_count;
    int max_equal_tabu_count;
    double start_time;
    double end_time;

public:
    Graph(int input_num_vertex, int input_edge_num, int input_num_color, vector<array<int, 2>>& input_edges, int seed);
    ~Graph();

    void find_move();
    void make_move();
    void tabu_search();

    void print_graph() const; // print adjacent list of graph;
    int get_solution(int i);

    // debug function
    int get_max_equal_nontabu_count();
    int get_max_equal_tabu_count();
};

#endif //SDK_GCP_TABUCOL_H