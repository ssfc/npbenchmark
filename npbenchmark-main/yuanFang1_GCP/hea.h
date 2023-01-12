//
// Created by take_ on 2023/1/4.
//

#ifndef YUANFANG1_GCP_HEA_H
#define YUANFANG1_GCP_HEA_H

#include <algorithm>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#define MaxPoint 1002
#define MaxColor 300


struct ArcNode {
    int adj_vertex;
    struct ArcNode *next;
};

struct VerNode {
    ArcNode *first;
};

struct Move {
    int u, vi, vj;
};

class Population_solution {
public:
    int psol[MaxColor][MaxPoint];
    int color_num[MaxColor];
    int index1[MaxPoint];
    int index2[MaxPoint];

    Population_solution();
    ~Population_solution();
};

class Population {
public:
    int min_conflict; // the min conflict among the population collection;
    int min_conflict_index;
    int *num_conflict;

    Population(int input_num_population);
    ~Population();
};

// class HE;
class Hybrid_Evolution
{
private:
    int iter;

public:
    // variables;
    int num_vertex;
    int num_color;

    // VerNode *adj_list;
    vector<VerNode> adj_list;
    int adj_color_table[MaxPoint][MaxPoint];
    //int **adj_color_table;


    int best_conflict;
    int f;
    int conflict_num;
    int *conflicts;
    int *conflict_index;

    int num_population;
    int **solution_collection;
    Population_solution *population_solution;

    int max_iter;


    // functions
    Hybrid_Evolution(int input_num_vertex, int input_num_color, int input_num_population);
    ~Hybrid_Evolution();

    void insert_adj_list(int i, int j);
    void add_conflict(int adj_vertex);
    void delete_conflict(int adj_vertex);

    int tabu_search(int *solution);
    Move find_move(const int *s);
    void make_move(int u, int vj, int *solution);

    void cross_over(int p1, int p2, int *index1) const;

    // debug function: compute conflict of a solution
    int compute_conflict(const int *solution);
};


#endif //YUANFANG1_GCP_HEA_H
