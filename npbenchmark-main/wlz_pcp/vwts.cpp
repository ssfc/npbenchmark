//
// Created by take_ on 2023/3/4.
//
#include "vwts.h"
#include <climits>
#include <ctime>
#include <vector>

using namespace std;

VWTS::VWTS(int input_num_vertex, int input_num_center, int input_radius,
           std::vector<std::vector<int>>& input_coverages,
           int input_seed)
{
    init_rand(input_seed); // initialize random generator;;

    num_vertex = input_num_vertex;
    num_center = input_num_center;
    current_radius = input_radius;

    num_center_cover.resize(num_vertex, 0);
    center_coverages = new int* [num_vertex];

    for (int i = 0; i < num_vertex; i++)
    {
        int count_client = input_coverages[i].size();
        num_center_cover[i] = count_client;
        center_coverages[i] = new int[count_client];

        for (int j = 0; j < count_client; j++)
            center_coverages[i][j] = input_coverages[i][j];
    }

    solution = new bool[num_vertex];
    center.resize(num_center, 0);
    num_covered_center = new int[num_vertex];
    covered_once = new int[num_vertex];
    uncovered_vertices = new int[num_vertex];

    // A1 LINE 3: /* (Section 3.2) */
    // for all i属于V do
    //    Vertex weights wi <- 1,
    // i: 顶点序号;
    // wi: vertex weights;
    // V: vertex set;
    vertex_weights.resize(num_vertex, 1);

    sum_uncovered_weight = min_history_sum_uncovered_weight = num_uncovered = num_vertex;
    center_weights.resize(num_vertex, 0);
    tabu_open = tabu_close = -1;
    min_delta = INT_MAX;
    for (int i = 0; i < num_vertex; i++)
    {
        solution[i] = false;//一开始没有被挑选为中心的点
        num_covered_center[i] = 0;//每个点都没有被中心覆盖
    }

    // debug variables;
    start_time = clock();
}

VWTS::~VWTS()
{
    for (int i = 0; i < num_vertex; i++)
        delete[] center_coverages[i];
    delete[] solution;
    delete[] num_covered_center;
    delete[] covered_once;
    delete[] uncovered_vertices;
}

void VWTS::greedy_construct()
{
    int max_uncovered;//记录最多能覆盖的未被覆盖节点数
    int cur_uncovered;//当前集合能覆盖的未覆盖元素数目
    vector<int> best_list;//记录能够覆盖最多未覆盖节点的节点列表
    for (int ip = 0; ip < num_center; ip++)
    {
        max_uncovered = 0;
        best_list.clear();
        for (int inum = 0; inum < num_vertex; inum++)
        {
            //如果未被选为中心就检查
            if (!solution[inum])//从未选为中心的结点中找到最好的那些加入列表
            {
                //计算当前节点能覆盖多少未被覆盖的节点
                int res = 0;
                for (int i = 0; i < num_center_cover[inum]; i++)
                    if (num_covered_center[center_coverages[inum][i]] == 0)
                        res++;
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
        int choose = best_list[generated_random() % best_list.size()];
        solution[choose] = true;
        for (int i = 0; i < num_center_cover[choose]; i++)
        {
            if (num_covered_center[center_coverages[choose][i]] == 0)//如果还未被别的中心覆盖
            {
                --sum_uncovered_weight;//加入节点
                --min_history_sum_uncovered_weight;
                --num_uncovered;
                covered_once[center_coverages[choose][i]] = choose;//表示仅被一个中心覆盖的节点
            }
            num_covered_center[center_coverages[choose][i]]++;//该元素被中心覆盖数++
        }
    }
    // 更新未覆盖的节点和中心
    num_uncovered = 0;
    int isolx = 0;
    for (int i = 0; i < num_vertex; i++)
    {
        if (solution[i]) //如果被选为中心，在中心列表中记录
            center[isolx++] = i;
        else if (num_covered_center[i] == 0)//如果是客户并且未被覆盖，在未覆盖节点中记录
            uncovered_vertices[num_uncovered++] = i;
    }
    //初始化delta（一开始权重都为1，所以用数量来代替权重和）
    init_delta();
}

// Algorithm 1;
// Input symbol:
// G: graph;
// p: center number;
// rq: covering radius;
// Output symbol:
// X*: history best solution;
void VWTS::vertex_weight_tabu_search()
{
    // A1 LINE 1: /* (Section 3.1) */
    // X <- init(G, p, rq);
    // X: initial solution generated;
    // G: graph;
    // p: centers;
    // rq: 最小化的最大服务半径;
    // Meaning: generates an initial solution X by a greedy algorithm; (2023年2月10日)
    greedy_construct();

    // A1 LINE 2:
    // X* <- X,
    // X*: history best solution;
    // X: initial solution generated (a set of centers);
    int best_num_uncovered = num_uncovered;

    // X_prev <- X;
    // X_prev: solution of the previous iteration;
    // X: initial solution generated;
    int prev_num_uncovered = INT_MAX;

    // iter <- 1;
    // iter: current iteration;
    int iter = 1;

    Move moved{0, 0};

    // A1 LINE 4:
    // while termination condition is not met do
    // Meaning: iteratively improves the incumbent solution by a tabu search procedure; (2023年2月10日)
    while (num_uncovered != 0)
    {
        // A1 LINE 5:
        // (i, j) <- FindPair(X_prev, TL, iter) /* (Algorithm 2) */
        // (i, j): pair swapped
        // X_prev: solution of the previous iteration;
        // TL: tabu list;
        // iter: current iteration;
        // Meaning: find_move; evaluates the neighborhood of the current solution and records the best neighborhood move while respecting their tabu states; (2023年2月10日)
        find_pair(moved.center_in, moved.center_out);
        if (moved.center_in == -1 || moved.center_out == -1)//没找到非禁忌move，解除禁忌进行下一轮
            continue;

        // A1 LINE 6:
        // MakeMove(i, j) /* (Algorithm 4) */
        // (i,j): pair moved found in the previous;
        // Meaning: makes the best move; (2023年2月10日)
        make_move(moved.center_in, moved.center_out);//进行move并比较

        // A1 LINE 7:
        // if |U(X)| < |U(X*)| then
        // X: current solution;
        // |U(X)|: the set of clients uncovered by X;
        // X*: history best solution;
        // |U(X*)|: the set of clients uncovered by X*;
        // Meaning: If the current solution X improves the best solution found so far
        if(num_uncovered < best_num_uncovered)
        {
            // A1 LINE 8:
            // X* <- X
            // X*: history best solution;
            // X: current solution;
            // Meaning: then X* is updated with X;
            best_num_uncovered = num_uncovered;
        }
        // A1 LINE 9:
        // else if |U(X)| >= |U(X_prev)| then
        // X: current solution;
        // |U(X)|: the set of clients uncovered by X;
        // X_prev: solution of the previous iteration;
        // |U(X')|: the set of clients uncovered by X';
        // Meaning: the best move returned by function FindPair() cannot reduce the number of uncovered clients; (2023年2月17日)
        else if(num_uncovered >= prev_num_uncovered)
        {
            // A1 LINE 10:
            /* (Section 3.2) */
            // for v belongs to U(X) do
            //    wv <- wv + 1
            // wv: weight of vertex that uncovered by X;
            // Meaning 1: If stagnation (停滞) occurs, the weight of each uncovered client is adjusted; (2023年2月17日)
            // Meaning 2: when the tabu search is **trapped in local optimal** solution X, the VWTS algorithm increases the weight wi of each uncovered client i属于U(X) by one unit; (2023年2月18日)
            // Meaning 3: prevent vertices from being repeatedly uncovered and diversify the search in an adaptive manner; (2023年2月18日)
            for (int i = 0; i < num_uncovered; i++)
            {
                int v = uncovered_vertices[i];
                vertex_weights[v]++;
                sum_uncovered_weight++;
                for (int ic = 0; ic < num_center_cover[v]; ic++)  //邻居delta值相应变大
                    center_weights[center_coverages[v][ic]]++ ;
            }

            // A1 LINE 11:
            // end if /* more uncovered clients than last solution */
        }

        // A1 LINE 12:
        // TL <- (i, j) /* update tabu list (Section 3.4) */
        // TL: tabu list;
        // (i, j): pairs found;
        // Meaning: update tabu list; (2023年2月17日)
        tabu_open = moved.center_out;
        tabu_close = moved.center_in;

        // A1 LINE 13:
        // X_prev <- X, iter <- iter + 1;
        // X_prev: solution of the previous iteration;
        // X: current solution;
        prev_num_uncovered = num_uncovered;
        iter++;

        if (iter % 100000 == 0)
        {
            cerr << "radius: " << current_radius << " iter: " << iter << " ";
            double elapsed_time = (clock() - start_time) / CLOCKS_PER_SEC;
            cerr << " elapsed time(s): " << elapsed_time
                 << " frequency:" << double(iter) / elapsed_time << endl;
        }

        // A1 LINE 14:
        // end while;
        // Meaning: when the specified termination condition is met, the algorithm terminates and returns the best solution X*; (2023年2月17日)
    }

    if (num_uncovered == 0)//更新
    {
        double elapsed_time = (clock() - start_time) / CLOCKS_PER_SEC;
        cerr << "success, radius: " << current_radius << " iter: " << iter
             << " elapsed_time(s): " << elapsed_time << " frequency:" << double (iter) / elapsed_time << endl;
        return;
    }
}

void VWTS::init_delta()
{
    for (int i = 0; i < num_vertex; i++)//o(n^2/p^2) ~ o(n^2)
    {
        int flag = solution[i] ? 1 : 0;
        //如果是非中心节点，delta为覆盖未覆盖元素数量
        //如果是中心，delta为仅由该中心覆盖元素数量
        center_weights[i] = 0;
        for (int j = 0; j < num_center_cover[i]; j++)
            if (num_covered_center[center_coverages[i][j]] == flag)
                center_weights[i]++;
    }
}

void VWTS::find_pair(int& v_open, int& v_close)
{
    int choose = uncovered_vertices[generated_random() % num_uncovered];
    min_delta = INT_MAX;
    vector<int> best_open, best_close;
    int* delta_p = new int[num_center];//记录中心delta，便于还原
    for (int i = 0; i < num_center; i++)
    {
        delta_p[i] = center_weights[center[i]];
    }
    for (int ic = 0; ic < num_center_cover[choose]; ic++)//o(n/p)，遍历能够覆盖未覆盖节点的点
    {
        int vc = center_coverages[choose][ic];//try_open结点(能够覆盖未覆盖节点的所有能覆盖的节点)
        //改变受影响中心的delta
        for (int jc = 0; jc < num_center_cover[vc]; jc++)//o(n/p)
        {
            int vjc = center_coverages[vc][jc];
            if (num_covered_center[vjc] == 1)//如果当前节点能被原有中心覆盖一次，再次覆盖就要更新值
                center_weights[covered_once[vjc]] -= vertex_weights[vjc];
        }
        //更新best_delta_f
        for (int ip = 0; ip < num_center; ip++)
        {
            int this_iter_delta = center_weights[center[ip]] - center_weights[vc];//加入节点vc

            if (vc != tabu_open && center[ip] != tabu_close || sum_uncovered_weight - this_iter_delta < min_history_sum_uncovered_weight)
            {
                if (this_iter_delta < min_delta)//重计最好值列表
                {
                    min_delta = this_iter_delta;
                    best_open.clear();
                    best_open.push_back(vc);
                    best_close.clear();
                    best_close.push_back(center[ip]);
                }
                else if (this_iter_delta == min_delta)//加入最好值列表
                {
                    best_open.push_back(vc);
                    best_close.push_back(center[ip]);
                }
            }
        }
        for (int ip = 0; ip < num_center; ip++)
        {
            center_weights[center[ip]] = delta_p[ip];
        }
    }

    delete[]delta_p;
    if (best_open.size() != 0)//随机选取一个开放
    {
        choose = generated_random() % best_open.size();
        v_open = best_open[choose];
        v_close = best_close[choose];
    }
    else
    {
        v_open = v_close = -1;
        tabu_open = tabu_close = -1;
    }
}

void VWTS::make_move(int v_open, int v_close)
{
    open_center(v_open);
    close_center(v_close);
    sum_uncovered_weight = sum_uncovered_weight + min_delta;
    if (sum_uncovered_weight < min_history_sum_uncovered_weight)
        min_history_sum_uncovered_weight = sum_uncovered_weight;

    //更新
    num_uncovered = 0;
    int isolx = 0;
    for (int i = 0; i < num_vertex; i++)
    {
        if (solution[i])//如果被选为中心，在中心列表中记录
            center[isolx++] = i;
        else if (num_covered_center[i] == 0)//如果是客户并且未被覆盖，在未覆盖节点中记录
            uncovered_vertices[num_uncovered++] = i;
    }
}

void VWTS::open_center(int i) //在X中加入中心i
{
    solution[i] = true;
    //delta[v] = 0;
    //更新邻域delta
    for (int ic = 0; ic < num_center_cover[i]; ic++)//o(n/p)
    {
        int vc = center_coverages[i][ic];
        if (num_covered_center[vc] == 1)//邻居vc原来唯一覆盖的中心delta--, o(1)
            center_weights[covered_once[vc]] -= vertex_weights[vc];
        else if (num_covered_center[vc] == 0)//新覆盖结点的邻居delta--
        {
            for (int jc = 0; jc < num_center_cover[vc]; jc++)
            {
                center_weights[center_coverages[vc][jc]] -= vertex_weights[vc];
            }
            covered_once[vc] = i;
            center_weights[i] += vertex_weights[vc];
        }
        num_covered_center[vc]++;
    }
}

void VWTS::close_center(int j)
{
    solution[j] = false;
    //更新邻域delta
    for (int ic = 0; ic < num_center_cover[j]; ic++)
    {
        int vc = center_coverages[j][ic];
        if (num_covered_center[vc] == 1)//vc变成未覆盖结点
        {
            for (int jc = 0; jc < num_center_cover[vc]; jc++)
                center_weights[center_coverages[vc][jc]] += vertex_weights[vc];
            center_weights[j] -= vertex_weights[vc];
        }
        else if (num_covered_center[vc] == 2)//vc周围中心变成唯一覆盖vc
        {
            for (int jc = 0; jc < num_center_cover[vc]; jc++)
                if (solution[center_coverages[vc][jc]])
                {
                    center_weights[center_coverages[vc][jc]] += vertex_weights[vc];
                    covered_once[vc] = center_coverages[vc][jc];//更新被唯一覆盖中心
                    break;
                }
        }
        --num_covered_center[vc];
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