//
// Created by take_ on 2023/3/4.
//

#ifndef WLZ_PCP_SOLUTION_H
#define WLZ_PCP_SOLUTION_H

#pragma once
#include<iostream>
using namespace std;

class VWTS
{
private:
    //图结构
    int* client_cover_num;//每个点能够覆盖的节点数
    int** elements;//每个点覆盖的节点
    int nums, P;//节点数量，中心数量

    //记录变量
    bool* solution;//是否挑选为中心（求解）
    int* center;//记录中心节点
    int* covered_center_num;//每个元素被多少个中心覆盖
    int* covered_by;//仅被一个中心覆盖的点
    int uncovered_num;//记录没有被覆盖的节点数，用作uncovered_list的下标
    int* uncovered_list;//记录没有被覆盖的节点
    int* weight;//每个节点权重，随着未覆盖时间增大逐渐增大
    int f, best_f, best_delta_f; //f函数，记录加权和
    int* delta;//记录结点i加入中心或者删除中心对f的影响
    int tabu_open, tabu_close;//禁忌期为1，记录禁忌元素

    //输出文件路径和时间限制
    string output_path;
public:
    int tempiter;//暂时用于统计迭代次数
    int tempnum;//记录uncovered_num
    int start_ms, end_ms, limit_s;

    //构造函数和析构函数
    VWTS(string path,string out);//读文件并初始化变量
    ~VWTS();
    //主要实现
    void Greedy();
    void Solve(int limit_s, int rand_seed);
    void FindSwap(int& v_open, int& v_close);//寻找交换对
    void SwapMove(int v_open, int v_close);
    //别的一些过程函数
    void InitialDelta();//初始化delta
    void Open(int v);//打开节点
    void Close(int v);//关闭节点
    bool OutputFile();//输出文件
};

#endif //WLZ_PCP_SOLUTION_H
