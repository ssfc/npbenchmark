//
// Created by take_ on 2022/12/13.
//

#ifndef SDK_PCP_PCP_VECTOR_H
#define SDK_PCP_PCP_VECTOR_H

# include <vector>
#include <boost/dynamic_bitset.hpp>

using namespace std;

struct Move {
    int center_in;
    int center_out;
};


class PCP_Vector
{
private: // member variables.
    int num_vertex;
    int num_center;
    vector<vector<int>> center_coverages;
    // Dimension: num_vertex * num_vertex
    // Element: true/false;
    // Meaning: if center i covers vertex j, center_cover_vertex[i][j] = true; else = false;
    vector<boost::dynamic_bitset<>> center_cover_vertex;

    // Dimension: num_vertex * num_vertex
    // Element: true/false;
    // Meaning: if vertex i can reach center j, vertex_reach_center[i][j] = true; else = false;
    vector<boost::dynamic_bitset<>> vertex_reach_center;

    vector<vector<int>> nodes_with_drops;

    vector<unsigned int> solution; // index of selected center; dimension, num_center;
    vector<int> dbs_solution; // centers selected;
    unsigned int conflict;
    unsigned int best_conflict;

    vector<int> universe; // all points;
    vector<int> covered; // points already covered by set;
    boost::dynamic_bitset<> dbs_covered;
    vector<int> uncovered; // points has not been covered by set;

    Move moved;
    vector<unsigned int> tabu_tenure_table; // tenure of each pair of swap node; dimension, num_node;
    int min_delta; // 移动增量
    vector<Move> equal_delta; //非禁忌相同delta值
    long long int iter;

    // debug variables:
    double start_time;

public: // member functions.
    PCP_Vector(int input_num_vertex, int input_num_center,
               vector<vector<int>>& input_coverages, vector<vector<int>> &input_nodesWithDrops, int input_seed);
    ~PCP_Vector();

    void swap_center();
    void find_move();
    void make_move();
    void local_search();
    unsigned int get_solution(int i);

    // debug functions;
    void print_tabu_tenure_table();
    long long int get_iteration();
};

#endif //SDK_PCP_PCP_VECTOR_H
