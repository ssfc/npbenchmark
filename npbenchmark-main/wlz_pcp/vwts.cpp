//
// Created by take_ on 2023/3/4.
//
#pragma warning(disable:4996)
#include "vwts.h"
#include <climits>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

VWTS::VWTS(int input_num_vertex, int input_num_center,
           std::vector<std::vector<int>>& input_coverages,
           std::vector<std::vector<int>> &input_nodesWithDrops,
           int input_seed)
{
    num_vertex = input_num_vertex;
    num_center = input_num_center;

    num_center_cover = new int[num_vertex];
    center_coverages = new int* [num_vertex];

    for (int i = 0; i < num_vertex; ++i)
    {
        int count_client = input_coverages[i].size();
        num_center_cover[i] = count_client;
        center_coverages[i] = new int[count_client];

        for (int j = 0; j < count_client; ++j)
            center_coverages[i][j] = input_coverages[i][j];
    }

    solution = new bool[num_vertex];
    center = new int[num_center];
    covered_center_num = new int[num_vertex];
    covered_once = new int[num_vertex];
    uncovered_vertices = new int[num_vertex];
    vertex_weights = new int[num_vertex];
    f = best_f = num_uncovered = num_vertex;
    delta = new int[num_vertex];
    tabu_open = tabu_close = -1;
    best_delta_f = INT_MAX;
    for (int i = 0; i < num_vertex; ++i)
    {
        solution[i] = false;//一开始没有被挑选为中心的点
        covered_center_num[i] = 0;//每个点都没有被中心覆盖
        vertex_weights[i] = 1;//初始权重全部为1
    }
}

VWTS::~VWTS()
{
    for (int i = 0; i < num_vertex; ++i)
        delete[] center_coverages[i];
    delete[] num_center_cover;
    delete[] solution;
    delete[] center;
    delete[] covered_center_num;
    delete[] covered_once;
    delete[] uncovered_vertices;
    delete[] vertex_weights;
    delete[] delta;
}

void VWTS::greedy_construct()
{
    int max_uncovered;//记录最多能覆盖的未被覆盖节点数
    int cur_uncovered;//当前集合能覆盖的未覆盖元素数目
    vector<int> best_list;//记录能够覆盖最多未覆盖节点的节点列表
    for (int ip = 0; ip < num_center; ++ip)
    {
        max_uncovered = 0;
        best_list.clear();
        for (int inum = 0; inum < num_vertex; ++inum)
        {
            //如果未被选为中心就检查
            if (!solution[inum])//从未选为中心的结点中找到最好的那些加入列表
            {
                //计算当前节点能覆盖多少未被覆盖的节点
                int res = 0;
                for (int i = 0; i < num_center_cover[inum]; ++i)
                    if (covered_center_num[center_coverages[inum][i]] == 0)
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
        solution[choose] = true;
        for (int i = 0; i < num_center_cover[choose]; ++i)
        {
            if (covered_center_num[center_coverages[choose][i]] == 0)//如果还未被别的中心覆盖
            {
                --f;//加入节点
                --best_f;
                --num_uncovered;
                covered_once[center_coverages[choose][i]] = choose;//表示仅被一个中心覆盖的节点
            }
            ++covered_center_num[center_coverages[choose][i]];//该元素被中心覆盖数++
        }
    }
    // 更新未覆盖的节点和中心
    num_uncovered = 0;
    int isolx = 0;
    for (int i = 0; i < num_vertex; ++i)
    {
        if (solution[i]) //如果被选为中心，在中心列表中记录
            center[isolx++] = i;
        else if (covered_center_num[i] == 0)//如果是客户并且未被覆盖，在未覆盖节点中记录
            uncovered_vertices[num_uncovered++] = i;
    }
    //初始化delta（一开始权重都为1，所以用数量来代替权重和）
    InitialDelta();
}

void VWTS::Solve(int rand_seed)
{
    start_ms = clock();
    srand(rand_seed);
    greedy_construct();//贪心

    int iter;
    int last_uncovered_num = INT_MAX, best_uncovered_num = num_uncovered;
    int v_open, v_close;

    if (num_uncovered == 0)//更新
    {
        end_ms = clock();
        tempiter = 0;
        tempnum = num_uncovered;
        return;
    }
    for (iter = 1; num_uncovered != 0; ++iter)
    {
        find_pair(v_open, v_close);
        if (v_open == -1 || v_close == -1)//没找到非禁忌move，解除禁忌进行下一轮
            continue;
        SwapMove(v_open, v_close);//进行move并比较
        if (num_uncovered < last_uncovered_num && num_uncovered < best_uncovered_num)
        {
            best_uncovered_num = num_uncovered;
        }
        else//比上次结果要坏，权重奖励未覆盖结点
        {
            for (int i = 0; i < num_uncovered; ++i)
            {
                int v = uncovered_vertices[i];
                ++vertex_weights[v], ++f;
                for (int ic = 0; ic < num_center_cover[v]; ++ic)  //邻居delta值相应变大
                    ++delta[center_coverages[v][ic]] ;
            }
        }
        last_uncovered_num = num_uncovered;
    }
    end_ms = clock();
    tempiter = iter;
    tempnum = num_uncovered;
}

void VWTS::InitialDelta()
{
    for (int i = 0; i < num_vertex; ++i)//o(n^2/p^2) ~ o(n^2)
    {
        int flag = solution[i] ? 1 : 0;
        //如果是非中心节点，delta为覆盖未覆盖元素数量
        //如果是中心，delta为仅由该中心覆盖元素数量
        delta[i] = 0;
        for (int j = 0; j < num_center_cover[i]; ++j)
            if (covered_center_num[center_coverages[i][j]] == flag)
                ++delta[i];
    }
}

void VWTS::find_pair(int& v_open, int& v_close)
{
    int choose = uncovered_vertices[rand() % num_uncovered];
    best_delta_f = INT_MAX;
    vector<int> best_open, best_close;
    int* delta_p = new int[num_center];//记录中心delta，便于还原
    for (int i = 0; i < num_center; ++i)
    {
        delta_p[i] = delta[center[i]];
    }
    for (int ic = 0; ic < num_center_cover[choose]; ++ic)//o(n/p)，遍历能够覆盖未覆盖节点的点
    {
        int vc = center_coverages[choose][ic];//try_open结点(能够覆盖未覆盖节点的所有能覆盖的节点)
        //改变受影响中心的delta
        for (int jc = 0; jc < num_center_cover[vc]; ++jc)//o(n/p)
        {
            int vjc = center_coverages[vc][jc];
            if (covered_center_num[vjc] == 1)//如果当前节点能被原有中心覆盖一次，再次覆盖就要更新值
                delta[covered_once[vjc]] -= vertex_weights[vjc];
        }
        //更新best_delta_f
        for (int ip = 0; ip < num_center; ++ip)
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
        for (int ip = 0; ip < num_center; ++ip)
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

void VWTS::SwapMove(int v_open, int v_close)
{
    Open(v_open);
    Close(v_close);
    f = f + best_delta_f;
    if (f < best_f)best_f = f;
    tabu_open = v_close;
    tabu_close = v_open;
    //更新
    num_uncovered = 0;
    int isolx = 0;
    for (int i = 0; i < num_vertex; ++i)
    {
        if (solution[i])//如果被选为中心，在中心列表中记录
            center[isolx++] = i;
        else if (covered_center_num[i] == 0)//如果是客户并且未被覆盖，在未覆盖节点中记录
            uncovered_vertices[num_uncovered++] = i;
    }
}

void VWTS::Open(int v)//加入结点v作为中心
{
    solution[v] = true;
    //delta[v] = 0;
    //更新邻域delta
    for (int ic = 0; ic < num_center_cover[v]; ++ic)//o(n/p)
    {
        int vc = center_coverages[v][ic];
        if (covered_center_num[vc] == 1)//邻居vc原来唯一覆盖的中心delta--, o(1)
            delta[covered_once[vc]] -= vertex_weights[vc];
        else if (covered_center_num[vc] == 0)//新覆盖结点的邻居delta--
        {
            for (int jc = 0; jc < num_center_cover[vc]; ++jc)
            {
                delta[center_coverages[vc][jc]] -= vertex_weights[vc];
            }
            covered_once[vc] = v;
            delta[v] += vertex_weights[vc];
        }
        ++covered_center_num[vc];
    }
}

void VWTS::Close(int v)
{
    solution[v] = false;
    //更新邻域delta
    for (int ic = 0; ic < num_center_cover[v]; ++ic)
    {
        int vc = center_coverages[v][ic];
        if (covered_center_num[vc] == 1)//vc变成未覆盖结点
        {
            for (int jc = 0; jc < num_center_cover[vc]; ++jc)
                delta[center_coverages[vc][jc]] += vertex_weights[vc];
            delta[v] -= vertex_weights[vc];
        }
        else if (covered_center_num[vc] == 2)//vc周围中心变成唯一覆盖vc
        {
            for (int jc = 0; jc < num_center_cover[vc]; ++jc)
                if (solution[center_coverages[vc][jc]])
                {
                    delta[center_coverages[vc][jc]] += vertex_weights[vc];
                    covered_once[vc] = center_coverages[vc][jc];//更新被唯一覆盖中心
                    break;
                }
        }
        --covered_center_num[vc];
    }
}

// output solution;
void VWTS::get_solution(vector<NodeId>& output)
{
    for (int i = 0; i < num_center; i++)
    {
        output[i] = center[i];
    }
}