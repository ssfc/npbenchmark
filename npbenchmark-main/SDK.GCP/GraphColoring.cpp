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
        int** adj_list; // adjacency list; 
        int* vertex_edge; // number of edge of each vertex; 

        //---
        //�����㷨
        int* solution; //����Ӧ��ɫ
        int conflict; //��ͻֵ
        int** tabu_tenure; //���ɱ�
        int** adj_color_table; //�ڽ���ɫ��
        int num_color; //��ɫ����
        int delta; //�ƶ�����
        int best_conflict; //��ʷ��õĳ�ͻֵ
        int node; //ÿ���ƶ��Ľ��
        int color; //ÿ���ƶ�����ɫ
        int iter; //��������

        int equ_delta[2000][2];//�ǽ�����ͬdeltaֵ
        int equ_tabudelta[2000][2];//������ͬdeltaֵ

    public:
        void split(const string& src, const string& delim, vector<string>& dest);
        void init_graph();
        void create_graph(string path);
        void initalloc();
        void delete_alloc();
        void initialization(int seed);

        void print_graph(); // print adjacent list of graph; 
        void find_move();
        void make_move();
        void tabu_search();
    };

    // class: ���ո��з�ÿ��
    void Graph::split(const string& src, const string& delim, vector<string>& dest)
    {
        dest.clear();
        string str = src;
        string::size_type start = 0, index;
        string substr;
        index = str.find(delim, start);    //��str�в���(��ʼ��start) delim�������ַ��ĵ�һ�γ��ֵ�λ��  
        while (index != string::npos)
        {
            substr = str.substr(start, index - start);
            dest.push_back(substr);
            start = index + 1;
            index = str.find(delim, start);    //��str�в���(��ʼ��index) ��һ��������delim���ַ����ֵ�λ��  
        }
        substr = str.substr(start, index);
        dest.push_back(substr);
    }

    // class: ��ʼ��ͼ
    void Graph::init_graph()
    {
        try
        {
            adj_list = new int* [num_vertex];//��ʼ��ͼ
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
            cerr << "ͼ�ڴ����ʧ��" << e.what() << endl;
            init_graph();//����ʧ�����·���
        }
    }

    //��ȡ�ļ����ݣ�����ͼ
    void Graph::create_graph(string path)
    {
        ifstream infile(path, ios::in);

        vector<string> data;
        string delim(" ");
        string textline;
        bool start = false;
        int v1, v2;
        int tmp;

        cerr << "Start reading graph..." << endl;

        if (infile.good())
        {
            while (!infile.fail())
            {
                getline(infile, textline);

                split(textline, delim, data);

                if (data.size() == 2)
                {
                    v1 = stoi(data[0]);
                    v2 = stoi(data[1]);

                    tmp = ++vertex_edge[v1];
                    adj_list[v1][tmp - 1] = v2;
                    tmp = ++vertex_edge[v2];
                    adj_list[v2][tmp - 1] = v1;
                }
                else if (data.size() == 3)
                {
                    num_vertex = stoi(data[0]);
                    num_color = stoi(data[2]);
                    init_graph();
                    start = true;
                }
            }
        }
        infile.close();

        cerr << "Finish reading graph." << endl;
    }

    // class: �����ڴ�; 
    void Graph::initalloc()
    {
        try
        {
            solution = new int[num_vertex];
            adj_color_table = new int* [num_vertex];
            tabu_tenure = new int* [num_vertex];

            for (int i = 0; i < num_vertex; i++)
            {
                adj_color_table[i] = new int[num_color];
                tabu_tenure[i] = new int[num_color];
            }

            for (int i = 0; i < num_vertex; i++)
            {
                for (int j = 0; j < num_color; j++)
                {
                    adj_color_table[i][j] = 0;
                    tabu_tenure[i][j] = 0;
                }
            }
        }
        catch (const bad_alloc& e)
        {
            cerr << "��ʼ���ڴ����ʧ��:" << e.what() << endl;
            initalloc();
        }
    }

    // �ͷ��ڴ�
    void Graph::delete_alloc()
    {
        for (int i = 0; i < num_vertex; i++)
        {
            delete[] tabu_tenure[i];
            delete[] adj_color_table[i];
            delete[] adj_list[i];
        }
        delete[] solution;
        delete[] tabu_tenure;
        delete[] adj_color_table;
        delete[] adj_list;
    }

    //��ʼ�������鶥����ɫ�������ʼ��ͻֵ����ʼ���ڽ���ɫ��
    void Graph::initialization(int seed)
    {
        conflict = 0;
        initalloc();//��ʼ���ڴ����

        srand(seed);
        for (int i = 0; i < num_vertex; i++)
            solution[i] = rand() % num_color;//��ʼ����ɫ

        /*
        cerr << "initial solution: ";
        for (int i = 0; i < num_vertex; i++)
            cerr << solution[i] << " ";
        cerr << endl;
        */

        int num_edge;
        int* h_graph;
        int adj_color;

        for (int i = 0; i < num_vertex; i++)
        {
            num_edge = vertex_edge[i];
            h_graph = adj_list[i];
            int this_vertex_color = solution[i];
            for (int u = 0; u < num_edge; u++)
            {
                adj_color = solution[h_graph[u]];
                if (this_vertex_color == adj_color) conflict++;
                adj_color_table[i][adj_color]++;//��ʼ���ڽ���ɫ��
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

    // class: ����ѽ��ɻ��߷ǽ����ƶ�
    void Graph::find_move()
    {
        delta = 10000; //��ʼΪ�������
        int tmp; //��ʱ����
        int tabu_delta = 10000;
        int count = 0, tabu_count = 0;
        int A = best_conflict - conflict;
        int* h_color; //�ڽ���ɫ������ָ��
        int* h_tabu; //���ɱ�����ָ��
        int c_color_table; //��ǰ�����ɫ���ֵ

        for (int i = 0; i < num_vertex; i++)
        {//11.3
            int this_vertex_color = solution[i];//6.1%
            h_color = adj_color_table[i];
            c_color_table = h_color[this_vertex_color];
            if (h_color[this_vertex_color] > 0)
            {//17.6
                h_tabu = tabu_tenure[i];
                for (int j = 0; j < num_color; j++)
                {
                    if (this_vertex_color != j)
                    {//cpu��ˮ��
                        //�ǽ����ƶ�
                        tmp = h_color[j] - c_color_table;
                        if (h_tabu[j] <= iter)
                        {//22.6
                            if (tmp <= delta)
                            {//��֧Ԥ�гͷ� 6.0
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
                        {//�����ƶ�
                            if (tmp <= tabu_delta)
                            {//6.0
                                if (tmp < tabu_delta)
                                {
                                    tabu_delta = tmp;
                                    tabu_count = 0;
                                }

                                tabu_count++;
                                equ_tabudelta[tabu_count - 1][0] = i;
                                equ_tabudelta[tabu_count - 1][1] = j;
                            }
                        }
                    }
                }
            }
        }

        tmp = 0;
        if (tabu_delta < A && tabu_delta < delta)
        {
            delta = tabu_delta;
            tmp = rand() % tabu_count;//���delta���ѡ��
            node = equ_tabudelta[tmp][0];
            color = equ_tabudelta[tmp][1];
        }
        else
        {
            tmp = rand() % count;//���delta���ѡ��
            node = equ_delta[tmp][0];
            color = equ_delta[tmp][1];
        }
    }

    // class: ����ֵ
    void Graph::make_move()
    {
        conflict = delta + conflict;//���³�ͻֵ

        if (conflict < best_conflict)
            best_conflict = conflict;//������ʷ��ó�ͻ

        int old_color = solution[node];
        solution[node] = color;
        tabu_tenure[node][old_color] = iter + conflict + rand() % 10 + 1;//���½��ɱ�
        int* h_graph = adj_list[node];
        int num_edge = vertex_edge[node];
        int tmp;

        for (int i = 0; i < num_edge; i++)
        {//�����ڽ���ɫ��
            tmp = h_graph[i];
            adj_color_table[tmp][old_color]--;
            adj_color_table[tmp][color]++;
        }
    }

    // class: ��������
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
            if ((iter % 100000) == 0)
                ofile << iter << " " << conflict << " " << num_color << " " << delta << " " << best_conflict << endl;
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

        // test.create_graph("./data/DSJC0250.9.txt");

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
