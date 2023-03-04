//
// Created by take_ on 2023/3/4.
//

#ifndef WLZ_PCP_VWTS_H
#define WLZ_PCP_VWTS_H

#pragma once
#include <iostream>
#include <vector>

using namespace std;
using NodeId = int;

class VWTS
{
private:
    //图结构
    int num_vertex;
    int num_center; //中心数量
    int** center_coverages; //每个中心覆盖的节点
    int* num_center_cover; //每个中心能够覆盖的节点数

    //记录变量
    bool* solution; //是否挑选为中心（求解）
    int* center; //记录中心节点
    int* covered_center_num; //每个顶点被多少个中心覆盖
    int* covered_once; //仅被一个中心覆盖的顶点
    int num_uncovered; //没有被覆盖的节点数，用作uncovered_list的下标
    int* uncovered_vertices; //记录没有被覆盖的节点
    int* vertex_weights; //每个节点权重，随着未覆盖时间增大逐渐增大

    int f;
    int best_f;
    int best_delta_f; //f函数，记录加权和

    int* delta; //记录中心i加入X或者删除对f的影响
    int tabu_open;
    int tabu_close; //禁忌期为1，记录禁忌元素
public:
    int tempiter;//暂时用于统计迭代次数
    int tempnum;//记录uncovered_num
    int start_ms, end_ms, limit_s;

    //构造函数和析构函数
    VWTS(int input_num_vertex, int input_num_center,
         std::vector<std::vector<int>>& input_coverages,
         int input_seed);//读文件并初始化变量
    ~VWTS();
    //主要实现
    void greedy_construct();
    void vertex_weight_tabu_search(int rand_seed);
    void find_pair(int& v_open, int& v_close);//寻找交换对
    void make_move(int v_open, int v_close);
    //别的一些过程函数
    void InitialDelta();//初始化delta
    void Open(int v);//打开节点
    void Close(int v);//关闭节点
    void get_solution(std::vector<NodeId>& output);
};

#endif //WLZ_PCP_VWTS_H
