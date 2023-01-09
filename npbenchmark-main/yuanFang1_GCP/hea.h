//
// Created by take_ on 2023/1/4.
//

#ifndef YUANFANG1_GCP_HEA_H
#define YUANFANG1_GCP_HEA_H

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

struct Population {
    int min_conflict; // the min conflict among the population collection;
    int min_conflict_index;
    int num_conflict[num_population];
};


class Hybrid_Evolution
{
private:

public:
    void dynamic_alloc();
    void insert_adj_list(int i, int j);

    int tabu_search(int *solution);
    Move find_move(int *s);




    // debug function: compute conflict of a solution
    int compute_conflict(int *solution);
};


#endif //YUANFANG1_GCP_HEA_H
