//
// Created by take_ on 2023/1/25.
//

#ifndef YUANFANG_REDUCE_HEA_H
#define YUANFANG_REDUCE_HEA_H

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
    int u;
    int vj;
};

class Population_solution {
public:
    vector<vector<int>> psol;
    vector<int> color_num;
    vector<unsigned int> index1s;
    vector<unsigned int> index2s;

    Population_solution(int input_num_color);
    ~Population_solution();
};

class Population {
public:
    int min_conflict; // the min conflict among the population collection;
    int min_conflict_index;
    vector<int> num_conflict;

    Population(int input_num_population);
    ~Population();
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

    vector<VerNode> adj_list;
    vector<vector<int>> adj_color_table;
    vector<vector<long long int>> tabu_tenure_table;

    int best_conflict;
    int conflict;
    int conflict_num;
    vector<int> conflicts;
    vector<int> conflict_index;

    int num_population;
    vector<vector<unsigned int>> solution_collection;  // dim, num_population * (num_vertex+1)
    vector<Population_solution> population_solution;

    Move moved;
    long long int max_iter;


    // functions
    Hybrid_Evolution(int input_num_vertex, int input_num_color, int input_num_population, int input_seed);
    ~Hybrid_Evolution();
    void insert_adj_list(int i, int j);

    void find_move(vector<unsigned int> &solution);
    void make_move(vector<unsigned int> &solution);
    void tabu_search(vector<unsigned int> &solution, bool is_limit);
    void add_conflict(int adj_vertex); // only used in make_move;
    void delete_conflict(int adj_vertex); // only used in make_move;

    void cross_over(unsigned int p1, unsigned int p2, vector<unsigned int> &index1);

    // debug function: compute conflict of a solution
    int compute_conflict(vector<unsigned int> &solution);
    [[nodiscard]] long long int get_iteration() const;
};


#endif //YUANFANG_REDUCE_HEA_H
