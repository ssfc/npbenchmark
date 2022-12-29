//
// Created by take_ on 2022/12/13.
//

#ifndef SDK_PCP_PCP_VECTOR_H
#define SDK_PCP_PCP_VECTOR_H

# include <vector>

using namespace std;

class PCP_Vector
{
private: // member variables.
    int seed;
    int num_node;
    int num_center;
    vector<vector<int>> center_coverages;
    vector<vector<int>> nodes_with_drops;

    unsigned int* solution; // index of selected center; dimension, num_center;
    unsigned int conflict;
    unsigned int best_conflict;
    int min_delta; // 移动增量

    vector<int> universe; // all points;
    vector<int> covered; // points already covered by set;
    vector<int> uncovered; // points has not been covered by set;
    int center_in;
    int center_out;

    unsigned int** tabu_tenure_table; // tenure of each pair of swap node; dimension, num_node * num_node;
    int equal_nontabu_delta[2000][2]; //非禁忌相同delta值
    int equal_tabu_delta[2000][2]; //禁忌相同delta值
    int iter;

public: // member functions.
    PCP_Vector(int input_nodeNum, int input_centerNum, vector<vector<int>>& input_coverages, vector<vector<int>> &input_nodesWithDrops, int input_seed);
    ~PCP_Vector();

    void swap_center();
    void find_move();
    void make_move();
    int local_search();
    unsigned int get_solution(int i);

    // debug functions;
    void print_tabu_tenure_table();
};

#endif //SDK_PCP_PCP_VECTOR_H
