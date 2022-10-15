#include "GraphColoring.h"

#include <climits>
#include <fstream>
#include <iostream>
#include <random>
#include <set>
#include <stack>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>

using namespace std;

namespace szx {

    class Graph
    {
    public:
        int num_vertex; // number of vertex in the graph; 
        int num_color; //颜色数量
        int** adj_list; // adjacency list; 
        int* vertex_edge; // number of edge of each vertex; 

        //---
        //禁忌算法
        int* solution; //结点对应颜色
        int conflict; //冲突值

        int** tabu_tenure_table; //禁忌表
        int** adjacent_color_table; // first dim is num vertex, second dim is num color; 

        int delta; //移动增量
        int best_conflict; //历史最好的冲突值
        int node_moved; //每次移动的结点
        int color_moved; //每次移动的颜色
        int iter; //迭代次数

        int equ_delta[2000][2]; //非禁忌相同delta值
        int equ_tabu_delta[2000][2]; //禁忌相同delta值

    public:
        void init_graph();
        void initalloc();
        void delete_alloc();
        void initialization(int seed);

        void print_graph(); // print adjacent list of graph; 
        void find_move();
        void make_move();
        void tabu_search();
    };

    // class: 初始化图
    void Graph::init_graph()
    {
        try
        {
            adj_list = new int* [num_vertex];//初始化图
            vertex_edge = new int[num_vertex];

            for (int i = 0; i < num_vertex; i++)
            {
                adj_list[i] = new int[num_vertex];
                vertex_edge[i] = 0;
            }

            for (int i = 0; i < num_vertex; i++)
                for (int j = 0; j < num_vertex; j++)
                    adj_list[i][j] = 0;
        }
        catch (const bad_alloc& e)
        {
            cerr << "图内存分配失败" << e.what() << endl;
            init_graph();//分配失败重新分配
        }
    }

    // class: 分配内存; 
    void Graph::initalloc()
    {
        try
        {
            solution = new int[num_vertex];
            adjacent_color_table = new int* [num_vertex];
            tabu_tenure_table = new int* [num_vertex];

            for (int i = 0; i < num_vertex; i++)
            {
                adjacent_color_table[i] = new int[num_color];
                tabu_tenure_table[i] = new int[num_color];
            }

            for (int i = 0; i < num_vertex; i++)
            {
                for (int j = 0; j < num_color; j++)
                {
                    adjacent_color_table[i][j] = 0;
                    tabu_tenure_table[i][j] = 0;
                }
            }
        }
        catch (const bad_alloc& e)
        {
            cerr << "初始化内存分配失败:" << e.what() << endl;
            initalloc();
        }
    }

    // 释放内存
    void Graph::delete_alloc()
    {
        for (int i = 0; i < num_vertex; i++)
        {
            delete[] tabu_tenure_table[i];
            delete[] adjacent_color_table[i];
            delete[] adj_list[i];
        }
        delete[] solution;
        delete[] tabu_tenure_table;
        delete[] adjacent_color_table;
        delete[] adj_list;
    }

    //初始化，分组顶点颜色，计算初始冲突值，初始化邻接颜色表
    void Graph::initialization(int seed)
    {
        conflict = 0;
        initalloc();//初始化内存分配

        srand(seed);
        for (int i = 0; i < num_vertex; i++)
            solution[i] = rand() % num_color;//初始化颜色

        /*
        cerr << "initial solution: ";
        for (int i = 0; i < num_vertex; i++)
            cerr << solution[i] << " ";
        cerr << endl;
        */

        int* h_graph;
        int adj_color;

        for (int i = 0; i < num_vertex; i++)
        {
            int num_edge = vertex_edge[i];
            h_graph = adj_list[i];
            int this_vertex_color = solution[i];

            for (int u = 0; u < num_edge; u++)
            {
                adj_color = solution[h_graph[u]];

                if (this_vertex_color == adj_color) 
                    conflict++;

                adjacent_color_table[i][adj_color]++; //初始化邻接颜色表
            }
        }

        conflict = conflict / 2; 
        best_conflict = conflict;
        cerr << "init number of confilcts:" << conflict << endl;
    }

    // class: print adjacency list of graph;
    void Graph::print_graph()
    {
        cerr << "Adjacency list of graph: " << num_vertex << " " << num_color << endl;
        for (int i = 0; i < num_vertex; i++)
        {
            for (int j = 0;j < vertex_edge[i];j++)
            {
                cerr << adj_list[i][j] << " ";
            }
            cerr << endl;
        }
    }

    // class: 找最佳禁忌或者非禁忌移动
    void Graph::find_move()
    {
        delta = INT_MAX; 
        int tabu_delta = INT_MAX;
        int count = 0;
        int tabu_count = 0;

        int A = best_conflict - conflict;

        int c_color_table; //当前结点颜色表的值

        for (int i = 0; i < num_vertex; i++)
        {
            int this_vertex_color = solution[i];
            int* h_color = adjacent_color_table[i];
            c_color_table = h_color[this_vertex_color];
            if (h_color[this_vertex_color] > 0)
            {//17.6
                int* h_tabu = tabu_tenure_table[i]; 
                for (int j = 0; j < num_color; j++)
                {
                    if (this_vertex_color != j)
                    {//cpu流水线
                        //非禁忌移动
                        int tmp = h_color[j] - c_color_table;
                        if (h_tabu[j] <= iter)
                        {
                            if (tmp <= delta)
                            {//分支预判惩罚 6.0
                                if (tmp < delta)
                                {
                                    count = 0;
                                    delta = tmp;
                                }
                                count++;
                                equ_delta[count - 1][0] = i;
                                equ_delta[count - 1][1] = j;
                            }
                        }
                        else
                        {//禁忌移动
                            if (tmp <= tabu_delta)
                            {//6.0
                                if (tmp < tabu_delta)
                                {
                                    tabu_delta = tmp;
                                    tabu_count = 0;
                                }

                                tabu_count++;
                                equ_tabu_delta[tabu_count - 1][0] = i;
                                equ_tabu_delta[tabu_count - 1][1] = j;
                            }
                        }
                    }
                }
            }
        }

        if (tabu_delta < A && tabu_delta < delta)
        {
            delta = tabu_delta;
            int tmp = rand() % tabu_count;//相等delta随机选择
            node_moved = equ_tabu_delta[tmp][0];
            color_moved = equ_tabu_delta[tmp][1];
        }
        else
        {
            int tmp = rand() % count;//相等delta随机选择
            node_moved = equ_delta[tmp][0];
            color_moved = equ_delta[tmp][1];
        }
    }

    // class: 更新值
    void Graph::make_move()
    {
        conflict = delta + conflict;//更新冲突值

        if (conflict < best_conflict)
            best_conflict = conflict;//更新历史最好冲突

        int old_color = solution[node_moved];
        solution[node_moved] = color_moved;
        tabu_tenure_table[node_moved][old_color] = iter + conflict + rand() % 10 + 1;//更新禁忌表

        int* h_graph = adj_list[node_moved];
        int num_edge = vertex_edge[node_moved];

        for (int i = 0; i < num_edge; i++)
        {//更新邻接颜色表
            int tmp = h_graph[i];
            adjacent_color_table[tmp][old_color]--;
            adjacent_color_table[tmp][color_moved]++;
        }
    }

    // class: 禁忌搜索
    void Graph::tabu_search()
    {
        ofstream ofile("total_O3.txt", ios::out);
        double start_time, end_time;
        double elapsed_time;

        start_time = clock();
        iter = 0;
        while (conflict > 0)
        {
            iter++;
            // cerr << "iter: " << iter << endl; 
            find_move();
            make_move();
        }

        end_time = clock();
        elapsed_time = (double(end_time - start_time)) / CLOCKS_PER_SEC;

        cerr << "success, iterations: " << iter << " elapsed_time(s): " << elapsed_time << " frequency:" << double(iter / elapsed_time) << endl;

        // save solutions; 
        for (int i = 0;i < num_vertex; i++)
        {
            ofile << solution[i] << endl;
        }

        ofile.close();
    }

class Solver {
	// random number generator.
	mt19937 pseudoRandNumGen;
	void initRand(int seed) { pseudoRandNumGen = mt19937(seed); }
	int fastRand(int lb, int ub) { return (pseudoRandNumGen() % (ub - lb)) + lb; }
	int fastRand(int ub) { return pseudoRandNumGen() % ub; }
	int rand(int lb, int ub) { return uniform_int_distribution<int>(lb, ub - 1)(pseudoRandNumGen); }
	int rand(int ub) { return uniform_int_distribution<int>(0, ub - 1)(pseudoRandNumGen); }

public:
	void solve(NodeColors& output, GraphColoring& input, std::function<bool()> isTimeout, int seed) {
		initRand(seed);

		// TODO: implement your own solver which fills the `output` to replace the following trivial solver.
		// sample solver: assign colors randomly (the solution can be infeasible).

		//                      +----[ exit before timeout ]
		//       
		
		/*               |
		for (NodeId n = 0; !isTimeout() && (n < input.nodeNum); ++n) 
		{ 
			output[n] = rand(input.colorNum); 
		}
		*/

		Graph test; 
        
        // create graph; 
        int v1, v2;
        int tmp;
        test.num_vertex = input.nodeNum;
        test.num_color = input.colorNum;
        test.init_graph();

        for (int i = 0;i < input.edgeNum; i++)
        {
            v1 = input.edges[i][0];
            v2 = input.edges[i][1];

            tmp = ++test.vertex_edge[v1];
            test.adj_list[v1][tmp - 1] = v2;
            tmp = ++test.vertex_edge[v2];
            test.adj_list[v2][tmp - 1] = v1;
        }

        // cerr << "Finish creating graph." << endl;

        // test.print_graph();

        test.initialization(seed);

        test.tabu_search();

        for (int i = 0;i < input.nodeNum;i++)
        {
            output[i] = test.solution[i];
        }

        test.delete_alloc();

	}
};

// solver.
void solveGraphColoring(NodeColors& output, GraphColoring& input, std::function<bool()> isTimeout, int seed) {
	Solver().solve(output, input, isTimeout, seed);
}

}
