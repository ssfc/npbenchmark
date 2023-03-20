//
// Created by take_ on 2022/12/13.
//

#ifndef SDK_PCP_PCP_VECTOR_H
#define SDK_PCP_PCP_VECTOR_H

#include <algorithm>
#include <climits>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include <boost/dynamic_bitset.hpp>

using NodeId = int;


class PCP_Vector
{
private: // member variables.
    int num_vertex;
    int num_center;
    int radius;
    std::vector<std::vector<int>> center_coverages;

    // Dimension: num_vertex * num_vertex
    // center_cover_vertex[i][j]: if center i covers vertex j, center_cover_vertex[i][j] = true; else = false;
    std::vector<boost::dynamic_bitset<>> center_cover_vertex;

    // Dimension: num_vertex * (num_center that a vertex can reach);
    // vertex_reach_center[i]: if vertex i can reach center j1, vertex_reach_center[i] = {j1, j2, j3...};
    // std::vector<std::vector<int>> vertex_reaching;
    // Dimension: num_vertex;
    // Element: norm
    // Meaning: number of centers that cover vertex i;
    std::vector<int> num_reach_solution;

    // Dimension: num_vertex;
    // reach_one_solution[i]: number of centers that cover vertex i;
    std::vector<int> reach_one_solution;

    std::vector<int> solution_value;
    boost::dynamic_bitset<> solution; // centers selected;
    std::vector<int> vertex_weights; // weight of each vertex; dimension, num_vertex;

    std::vector<int> uncovered_value;
    boost::dynamic_bitset<> uncovered_vertices;

    int num_uncovered;
    int best_num_uncovered;
    int prev_num_uncovered;

    std::pair<int, int> moved;
    int min_delta;
    int tabu_open;
    int tabu_close;

    // Dimension: num_vertex;
    // Meaning: element j is sum of the weights of the vertices which can only be served by center j.
    // Therefore, it can evaluate the importance of this center.
    std::vector<int> center_weights;

    // dimension: num_vertex;
    // meaning: element j is sum of the weights of the vertices which can only be served by center j.
    std::vector<int> prev_center_weights;
    std::vector<std::pair<int, int>> equal_pair;
    int equal_pair_count;

    int sum_uncovered_weight; // f(X), 记录未覆盖顶点的加权和, Eq (11);

    long long iter;

    // debug variables:
    std::mt19937 generated_random;
    // int max_equal_pair_count;
    double start_time;

public: // member functions.
    PCP_Vector(int input_num_vertex, int input_num_center, int input_radius,
               std::vector<std::vector<int>>& input_coverages,
               int input_seed);
    ~PCP_Vector();

    void greedy_construct();
    void find_pair();
    void make_move();
    void vertex_weight_tabu_search();
    void get_solution(std::vector<NodeId>& output);

    // debug functions;
    void init_rand(int seed) { generated_random = std::mt19937(seed); }
    void print_index1(const std::string& name, const boost::dynamic_bitset<>& dbs);
    void sequence_construct(); // construct random solution;
    long long int get_iteration() const;
    static void print_vector(const std::string& name, std::vector<int> &vect);
    void print_equal_pair();
    unsigned compute_sum_uncovered_weight();
};

#endif //SDK_PCP_PCP_VECTOR_H