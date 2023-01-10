//
// Created by take_ on 2023/1/4.
//

#ifndef YUANFANG1_GCP_HEA_H
#define YUANFANG1_GCP_HEA_H

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
#define num_population 20


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
    int index1[MaxPoint], index2[MaxPoint];

    Population_solution();
    ~Population_solution();
};

class Population {
public:
    int min_conflict; // the min conflict among the population collection;
    int min_conflict_index;
    int *num_conflict;

    Population();
    ~Population();
};

// class HE;
class Hybrid_Evolution
{
private:
    long long iter;

public:
    // variables;
    int num_vertex;
    int num_color;

    VerNode *adj_list;
    int adj_color_table[MaxPoint][MaxPoint];


    int best_conflict;
    int f;
    int conflict_num;

    int **solution_collection;

    int max_iter;


    // functions
    Hybrid_Evolution();
    ~Hybrid_Evolution();

    void dynamic_alloc();
    void insert_adj_list(int i, int j) const;
    void add_conflict(int adj_vertex);
    void delete_conflict(int adj_vertex);

    int tabu_search(int *solution);
    Move find_move(const int *s);
    void make_move(int u, int vj, int *solution);

    void cross_over(int p1, int p2, int *index1) const;

    // debug function: compute conflict of a solution
    int compute_conflict(const int *solution);
    void split(const string& src, const string& delim, vector<string>& dest);
};


#endif //YUANFANG1_GCP_HEA_H
