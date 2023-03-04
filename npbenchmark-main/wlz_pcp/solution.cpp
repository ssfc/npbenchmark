//
// Created by take_ on 2023/3/4.
//
#pragma warning(disable:4996)
#include "solution.h"
#include<iostream>
#include<climits>
#include<fstream>
#include <fstream>
#include <sstream>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<cmath>
using namespace std;

SolveStruct::SolveStruct(string path,string out)
{
    //读文件
    string temp;
    ifstream fin(path);
    if (!fin)
    {
        printf("file %s is opened fail\n", path.c_str());
    }
    getline(fin, temp);
    sscanf(temp.c_str(), "%d%d", &nums, &P);
    client_cover_num = new int[nums];
    elements = new int* [nums];

    int count_client;
    for (int i = 0; i < nums; ++i)
    {
        getline(fin, temp);
        sscanf(temp.c_str(), "%d", &count_client);
        client_cover_num[i] = count_client;
        elements[i] = new int[count_client];

        getline(fin, temp);
        auto current_list = elements[i];
        stringstream ss(temp);
        for (int j = 0; j < count_client; ++j)
            ss >> current_list[j];
    }
    fin.close();
    output_path = out;
    Solution = new bool[nums];
    center = new int[P];
    covered_center_num = new int[nums];
    covered_by = new int[nums];
    uncovered_list = new int[nums];
    weight = new int[nums];
    f = best_f = uncovered_num = nums;
    delta = new int[nums];
    tabu_open = tabu_close = -1;
    best_delta_f = INT_MAX;
    for (int i = 0; i < nums; ++i)
    {
        Solution[i] = false;//一开始没有被挑选为中心的点
        covered_center_num[i] = 0;//每个点都没有被中心覆盖
        weight[i] = 1;//初始权重全部为1
    }
}
SolveStruct::~SolveStruct()
{
    for (int i = 0; i < nums; ++i)
        delete[] elements[i];
    delete[] client_cover_num;
    delete[] Solution;
    delete[] center;
    delete[] covered_center_num;
    delete[] covered_by;
    delete[] uncovered_list;
    delete[] weight;
    delete[] delta;
}

void SolveStruct::Greedy()
{
    int max_uncovered;//记录最多能覆盖的未被覆盖节点数
    int cur_uncovered;//当前集合能覆盖的未覆盖元素数目
    vector<int> best_list;//记录能够覆盖最多未覆盖节点的节点列表
    for (int ip = 0; ip < P; ++ip)
    {
        max_uncovered = 0;
        best_list.clear();
        for (int inum = 0; inum < nums; ++inum)
        {
            //如果未被选为中心就检查
            if (!Solution[inum])//从未选为中心的结点中找到最好的那些加入列表
            {
                //计算当前节点能覆盖多少未被覆盖的节点
                int res = 0;
                for (int i = 0; i < client_cover_num[inum]; ++i)
                    if (covered_center_num[elements[inum][i]] == 0)
                        ++res;
                cur_uncovered = res;

                if (cur_uncovered == max_uncovered)//加入最好值列表
                {
                    best_list.push_back(inum);
                }
                else if (cur_uncovered > max_uncovered)//如果比之前找到的max值大，更新
                {
                    best_list.clear();
                    best_list.push_back(inum);//重计最好值列表
                    max_uncovered = cur_uncovered;
                }
            }
        }
        //从列表中随机选一个并更新
        int choose = best_list[rand() % best_list.size()];
        Solution[choose] = true;
        for (int i = 0; i < client_cover_num[choose]; ++i)
        {
            if (covered_center_num[elements[choose][i]] == 0)//如果还未被别的中心覆盖
            {
                --f;//加入节点
                --best_f;
                --uncovered_num;
                covered_by[elements[choose][i]] = choose;//表示仅被一个中心覆盖的节点
            }
            ++covered_center_num[elements[choose][i]];//该元素被中心覆盖数++
        }
    }
    //更新未覆盖的节点和中心
    uncovered_num = 0;
    int isolx = 0;
    for (int i = 0; i < nums; ++i)
    {
        if (Solution[i] == true)//如果被选为中心，在中心列表中记录
            center[isolx++] = i;
        else if (covered_center_num[i] == 0)//如果是客户并且未被覆盖，在未覆盖节点中记录
            uncovered_list[uncovered_num++] = i;
    }
    //初始化delta（一开始权重都为1，所以用数量来代替权重和）
    InitialDelta();
}

void SolveStruct::Solve(int limit_s, int rand_seed)
{
    start_ms = clock();
    this->limit_s = limit_s;
    srand(rand_seed);
    Greedy();//贪心

    int iter;
    int last_uncovered_num = INT_MAX, best_uncovered_num = uncovered_num;
    int v_open, v_close;

    if (uncovered_num == 0 && (clock() - start_ms) / 1000 < limit_s)//更新
    {
        end_ms = clock();
        tempiter = 0;
        tempnum = uncovered_num;
        OutputFile();
        return;
    }
    for (iter = 1; uncovered_num != 0 && (clock() - start_ms) / 1000 < limit_s; ++iter)
    {
        FindSwap(v_open, v_close);
        if (v_open == -1 || v_close == -1)//没找到非禁忌move，解除禁忌进行下一轮
            continue;
        SwapMove(v_open, v_close);//进行move并比较
        if (uncovered_num < last_uncovered_num && uncovered_num < best_uncovered_num)
        {
            best_uncovered_num = uncovered_num;
            OutputFile();
        }
        else//比上次结果要坏，权重奖励未覆盖结点
        {
            for (int i = 0; i < uncovered_num; ++i)
            {
                int v = uncovered_list[i];
                ++weight[v], ++f;
                for (int ic = 0; ic < client_cover_num[v]; ++ic)  //邻居delta值相应变大
                    ++delta[elements[v][ic]] ;
            }
        }
        last_uncovered_num = uncovered_num;
    }
    end_ms = clock();
    tempiter = iter;
    tempnum = uncovered_num;
}

void SolveStruct::InitialDelta()
{
    for (int i = 0; i < nums; ++i)//o(n^2/p^2) ~ o(n^2)
    {
        int flag = Solution[i] ? 1 : 0;
        //如果是非中心节点，delta为覆盖未覆盖元素数量
        //如果是中心，delta为仅由该中心覆盖元素数量
        delta[i] = 0;
        for (int j = 0; j < client_cover_num[i]; ++j)
            if (covered_center_num[elements[i][j]] == flag)
                ++delta[i];
    }
}

void SolveStruct::FindSwap(int& v_open, int& v_close)
{
    int choose = uncovered_list[rand() % uncovered_num];
    best_delta_f = INT_MAX;
    vector<int> best_open, best_close;
    int* delta_p = new int[P];//记录中心delta，便于还原
    for (int i = 0; i < P; ++i)
    {
        delta_p[i] = delta[center[i]];
    }
    for (int ic = 0; ic < client_cover_num[choose]; ++ic)//o(n/p)，遍历能够覆盖未覆盖节点的点
    {
        int vc = elements[choose][ic];//try_open结点(能够覆盖未覆盖节点的所有能覆盖的节点)
        //改变受影响中心的delta
        for (int jc = 0; jc < client_cover_num[vc]; ++jc)//o(n/p)
        {
            int vjc = elements[vc][jc];
            if (covered_center_num[vjc] == 1)//如果当前节点能被原有中心覆盖一次，再次覆盖就要更新值
                delta[covered_by[vjc]] -= weight[vjc];
        }
        //更新best_delta_f
        for (int ip = 0; ip < P; ++ip)
        {
            int cur_delta_f = delta[center[ip]] - delta[vc];//加入节点vc

            if (vc != tabu_open && center[ip] != tabu_close || f - cur_delta_f < best_f)
            {
                if (cur_delta_f < best_delta_f)//重计最好值列表
                {
                    best_delta_f = cur_delta_f;
                    best_open.clear();
                    best_open.push_back(vc);
                    best_close.clear();
                    best_close.push_back(center[ip]);
                }
                else if (cur_delta_f == best_delta_f)//加入最好值列表
                {
                    best_open.push_back(vc);
                    best_close.push_back(center[ip]);
                }
            }
        }
        for (int ip = 0; ip < P; ++ip)
        {
            delta[center[ip]] = delta_p[ip];
        }
    }
    delete[]delta_p;
    if (best_open.size() != 0)//随机选取一个开放
    {
        choose = rand() % best_open.size();
        v_open = best_open[choose];
        v_close = best_close[choose];
    }
    else
    {
        v_open = v_close = -1;
        tabu_open = tabu_close = -1;
    }
}

void SolveStruct::SwapMove(int v_open, int v_close)
{
    Open(v_open);
    Close(v_close);
    f = f + best_delta_f;
    if (f < best_f)best_f = f;
    tabu_open = v_close;
    tabu_close = v_open;
    //更新
    uncovered_num = 0;
    int isolx = 0;
    for (int i = 0; i < nums; ++i)
    {
        if (Solution[i] == true)//如果被选为中心，在中心列表中记录
            center[isolx++] = i;
        else if (covered_center_num[i] == 0)//如果是客户并且未被覆盖，在未覆盖节点中记录
            uncovered_list[uncovered_num++] = i;
    }
}

void SolveStruct::Open(int v)//加入结点v作为中心
{
    Solution[v] = true;
    //delta[v] = 0;
    //更新邻域delta
    for (int ic = 0; ic < client_cover_num[v]; ++ic)//o(n/p)
    {
        int vc = elements[v][ic];
        if (covered_center_num[vc] == 1)//邻居vc原来唯一覆盖的中心delta--, o(1)
            delta[covered_by[vc]] -= weight[vc];
        else if (covered_center_num[vc] == 0)//新覆盖结点的邻居delta--
        {
            for (int jc = 0; jc < client_cover_num[vc]; ++jc)
            {
                delta[elements[vc][jc]] -= weight[vc];
            }
            covered_by[vc] = v;
            delta[v] += weight[vc];
        }
        ++covered_center_num[vc];
    }
}

void SolveStruct::Close(int v)
{
    Solution[v] = false;
    //更新邻域delta
    for (int ic = 0; ic < client_cover_num[v]; ++ic)
    {
        int vc = elements[v][ic];
        if (covered_center_num[vc] == 1)//vc变成未覆盖结点
        {
            for (int jc = 0; jc < client_cover_num[vc]; ++jc)
                delta[elements[vc][jc]] += weight[vc];
            delta[v] -= weight[vc];
        }
        else if (covered_center_num[vc] == 2)//vc周围中心变成唯一覆盖vc
        {
            for (int jc = 0; jc < client_cover_num[vc]; ++jc)
                if (Solution[elements[vc][jc]])
                {
                    delta[elements[vc][jc]] += weight[vc];
                    covered_by[vc] = elements[vc][jc];//更新被唯一覆盖中心
                    break;
                }
        }
        --covered_center_num[vc];
    }
}

bool SolveStruct::OutputFile()
{
    ofstream fout(output_path);
    if (!fout)
    {
        printf("output error :%s\n",output_path.c_str());
        return false;
    }
    for (int i = 0; i < P; ++i)
    {
        fout << center[i] << " ";
    }
    return true;
}