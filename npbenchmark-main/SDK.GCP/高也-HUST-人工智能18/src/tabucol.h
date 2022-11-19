//
// Created by take_ on 2022/11/3.
//

#ifndef SDK_GCP_TABUCOL_H
#define SDK_GCP_TABUCOL_H


class Graph
{
public:
    int num_vertex; // number of vertex in the graph;
    int num_color; // number of color in the graph;
    int** adj_list; // adjacency list;
    int* vertex_edge; // number of edge of each vertex;

    int* solution; // color of each vertex;
    int conflict;

    int** tabu_tenure_table;
    int** adj_color_table; // first dim is num vertex, second dim is num color;

    int delta; // �ƶ�����
    int best_conflict; // ��ʷ��õĳ�ͻֵ
    int node_moved; // ÿ���ƶ��Ľ��
    int color_moved; //ÿ���ƶ�����ɫ
    int iter;

    int equ_delta[2000][2]; //�ǽ�����ͬdeltaֵ
    int equ_tabu_delta[2000][2]; //������ͬdeltaֵ

public:
    void init_graph();
    void allocate_memory();
    void free_memory() const;
    void initialization(int seed);

    void print_graph() const; // print adjacent list of graph;
    void find_move();
    void make_move();
    void tabu_search();
};

#endif //SDK_GCP_TABUCOL_H
