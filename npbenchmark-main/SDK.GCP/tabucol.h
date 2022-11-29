//
// Created by take_ on 2022/11/3.
//

#ifndef SDK_GCP_TABUCOL_H
#define SDK_GCP_TABUCOL_H

# include "GraphColoring.h"
using namespace szx;

class Graph
{
private:
    int num_vertex; // number of vertex in the graph;
    int num_color; // number of color in the graph;
    int** adj_list; // adjacency list;
    int* vertex_edge; // number of edge of each vertex;

    unsigned int* solution; // color of each vertex;
    unsigned int conflict;

    unsigned int** tabu_tenure_table;
    int** adj_color_table; // first dim is num vertex, second dim is num color;

    int delta; // 移动增量
    unsigned int best_conflict; // 历史最好的冲突值
    int node_moved; // 每次移动的结点
    int color_moved; //每次移动的颜色
    unsigned int iter;

    int equ_delta[2000][2]; //非禁忌相同delta值
    int equ_tabu_delta[2000][2]; //禁忌相同delta值

public:
    Graph(int input_num_vertex, int input_edge_num, int input_num_color, GraphColoring& input, int seed);
    ~Graph();

    void find_move();
    void make_move();
    unsigned int tabu_search();

    void print_graph() const; // print adjacent list of graph;
    unsigned int get_solution(int i);
};

#endif //SDK_GCP_TABUCOL_H
