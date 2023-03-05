//
// Created by take_ on 2023/3/4.
//

#ifndef WLZ_PCP_VWTS_H
#define WLZ_PCP_VWTS_H

#pragma once
#include <boost/dynamic_bitset.hpp>
#include <iostream>
#include <random>
#include <vector>

using namespace std;
using NodeId = int;

struct Move {
    int center_in;
    int center_out;
};

class VWTS
{
private:
    //图结构
    int num_vertex;
    int num_center; //中心数量
    int current_radius;
    int** center_coverages; //每个中心覆盖的节点
    vector<int> num_center_cover; //每个中心能够覆盖的节点数

    //记录变量
    boost::dynamic_bitset<> solution; //是否挑选为中心（求解）
    vector<int> center; //记录中心节点

    int* num_covered_center; //每个顶点被多少个中心覆盖
    int* covered_once; //仅被一个中心覆盖的顶点
    vector<int> uncovered_vertices; //记录没有被覆盖的节点
    int num_uncovered; //没有被覆盖的节点数，用作uncovered_vertices的下标

    vector<int> vertex_weights; //每个节点权重，随着未覆盖时间增大逐渐增大
    vector<int> center_weights; //记录X加入中心i或者删除中心i对f(X)的影响

    int tabu_open;
    int tabu_close; //禁忌期为1，记录禁忌元素

    int sum_uncovered_weight; // f(X), 记录未覆盖顶点的加权和, Eq (11);
    int min_history_sum_uncovered_weight; // f(X) 的历史最小值;
    int min_delta; // 最好的swap进行后, f(X)值的变化;

    // debug variables
    std::mt19937 generated_random;
    double start_time;

public:
    VWTS(int input_num_vertex, int input_num_center, int input_radius,
         std::vector<std::vector<int>>& input_coverages,
         int input_seed);//读文件并初始化变量
    ~VWTS();

    void greedy_construct();
    void vertex_weight_tabu_search();
    void find_pair(int& v_open, int& v_close);//寻找交换对
    void make_move(int v_open, int v_close);

    void init_delta();
    void open_center(int v);
    void close_center(int v);

    // debug function
    void init_rand(int seed) { generated_random = std::mt19937(seed); }
    void get_solution(std::vector<NodeId>& output);
};

#endif //WLZ_PCP_VWTS_H
