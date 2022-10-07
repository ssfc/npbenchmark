// Code: https://github.com/chenfengkg/tabucol/blob/master/tabu.cpp
// Blog: https://codeleading.com/article/96523487065/ 
//禁忌搜索解决图着色问题

#include<time.h>
#include<fstream>
#include<stdlib.h>
#include<stack>
#include<vector>
#include<climits>
#include<string>
#include<iostream>
using namespace std;

int num_vertex; // number of vertex in the graph; 
int** adj_list; // adjacency list; 
int* vertex_edge; // number of edge of each vertex; 

//---
//禁忌算法
int* solution; //结点对应颜色
int conflict; //冲突值
int** tabu_tenure; //禁忌表
int** adj_color_table; //邻接颜色表
int num_color; //颜色数量
int delta; //移动增量
int best_conflict; //历史最好的冲突值
int node; //每次移动的结点
int color; //每次移动的颜色
int iter; //迭代次数
int equ_delta[2000][2];//非禁忌相同delta值
int equ_tabudelta[2000][2];//禁忌相同delta值

class Graph
{
public:
    int num_vertex; // number of vertex in the graph; 
    int** adj_list; // adjacency list; 
    int* vertex_edge; // number of edge of each vertex; 

    //---
    //禁忌算法
    int* solution; //结点对应颜色
    int conflict; //冲突值
    int** tabu_tenure; //禁忌表
    int** adj_color_table; //邻接颜色表
    int num_color; //颜色数量
    int delta; //移动增量
    int best_conflict; //历史最好的冲突值
    int node; //每次移动的结点
    int color; //每次移动的颜色
    int iter; //迭代次数

    int equ_delta[2000][2];//非禁忌相同delta值
    int equ_tabudelta[2000][2];//禁忌相同delta值

    void split(const string& src, const string& delim, vector<string>& dest); 
    void init_graph(); 
    void create_graph(string path); 
    void initalloc(); 
    void delete_alloc(); 
    void initialization(); 
    void find_move(); 
    void make_move(); 
    void tabu_search(int seed, string path); 
};

//按空格切分每行
void split(const string& src, const string& delim, vector<string>& dest)
{
    dest.clear();
    string str = src;
    string::size_type start = 0, index;
    string substr;
    index = str.find(delim, start);    //在str中查找(起始：start) delim的任意字符的第一次出现的位置  
    while (index != string::npos)
    {
        substr = str.substr(start, index - start);
        dest.push_back(substr);
        start = index + 1;
        index = str.find(delim, start);    //在str中查找(起始：index) 第一个不属于delim的字符出现的位置  
    }
    substr = str.substr(start, index);
    dest.push_back(substr);
}

// class: 按空格切分每行
void Graph::split(const string& src, const string& delim, vector<string>& dest)
{
    dest.clear();
    string str = src;
    string::size_type start = 0, index;
    string substr;
    index = str.find(delim, start);    //在str中查找(起始：start) delim的任意字符的第一次出现的位置  
    while (index != string::npos)
    {
        substr = str.substr(start, index - start);
        dest.push_back(substr);
        start = index + 1;
        index = str.find(delim, start);    //在str中查找(起始：index) 第一个不属于delim的字符出现的位置  
    }
    substr = str.substr(start, index);
    dest.push_back(substr);
}

// 初始化图
void init_graph() 
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
        cout << "图内存分配失败" << e.what() << endl;
        init_graph();//分配失败重新分配
    }
}

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
        cout << "图内存分配失败" << e.what() << endl;
        init_graph();//分配失败重新分配
    }
}

//读取文件数据，创建图
void create_graph(string path) 
{
    // ifstream infile("C:\\wamp64\\www\\npbenchmark\\npbenchmark-main\\SDK.GCP\\tabu_search\\data\\DSJC0500.1.txt", ios::in);
    // ifstream infile("./data/DSJC0250.5.txt", ios::in);
    ifstream infile(path, ios::in);

    vector<string> data;
    string delim(" ");
    string textline;
    bool start = false;
    int v1, v2;
    int tmp;

    cout << "Start reading graph..." << endl;

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
            else if(data.size() == 3)
            {
                num_vertex = stoi(data[0]);
                num_color = stoi(data[2]);
                init_graph();
                start = true;
            }
        }
    }
    infile.close();

    cout << "Finish reading graph." << endl;
}

//读取文件数据，创建图
void Graph::create_graph(string path)
{
    // ifstream infile("C:\\wamp64\\www\\npbenchmark\\npbenchmark-main\\SDK.GCP\\tabu_search\\data\\DSJC0500.1.txt", ios::in);
    // ifstream infile("./data/DSJC0500.1.txt", ios::in);
    ifstream infile(path, ios::in);

    vector<string> data;
    string delim(" ");
    string textline;
    bool start = false;
    int v1, v2;
    int tmp;

    cout << "Start reading graph..." << endl;

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

    cout << "Finish reading graph." << endl;
}

// 分配内存; 
void initalloc() 
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
        cout << "初始化内存分配失败:" << e.what() << endl;
        initalloc();
    }
}

// class: 分配内存; 
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
        cout << "初始化内存分配失败:" << e.what() << endl;
        initalloc();
    }
}

// 释放内存
void delete_alloc() 
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

// 释放内存
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

//初始化，分组顶点颜色，计算初始冲突值，初始化邻接颜色表
void initialization() 
{
    conflict = 0;
    initalloc();//初始化内存分配
    for (int i = 0; i < num_vertex; i++)
        solution[i] = rand() % num_color;//初始化颜色
    int num_edge;
    int* h_graph;
    int adj_color;
    int c_color;
    for (int i = 0; i < num_vertex; i++) 
    {
        num_edge = vertex_edge[i];
        h_graph = adj_list[i];
        c_color = solution[i];
        for (int u = 0; u < num_edge; u++) 
        {
            adj_color = solution[h_graph[u]];
            if (c_color == adj_color) conflict++;
            adj_color_table[i][adj_color]++;//初始化邻接颜色表
        }
    }

    conflict = conflict / 2;
    best_conflict = conflict;
    cout << "init number of confilcts:" << conflict << endl;
}

//初始化，分组顶点颜色，计算初始冲突值，初始化邻接颜色表
void Graph::initialization()
{
    conflict = 0;
    initalloc();//初始化内存分配
    for (int i = 0; i < num_vertex; i++)
        solution[i] = rand() % num_color;//初始化颜色
    int num_edge;
    int* h_graph;
    int adj_color;
    int c_color;
    for (int i = 0; i < num_vertex; i++)
    {
        num_edge = vertex_edge[i];
        h_graph = adj_list[i];
        c_color = solution[i];
        for (int u = 0; u < num_edge; u++)
        {
            adj_color = solution[h_graph[u]];
            if (c_color == adj_color) conflict++;
            adj_color_table[i][adj_color]++;//初始化邻接颜色表
        }
    }

    conflict = conflict / 2;
    best_conflict = conflict;
    cout << "init number of confilcts:" << conflict << endl;
}

//找最佳禁忌或者非禁忌移动
void find_move() 
{
    delta = 10000;//初始为最大整数
    int tmp;//临时变量
    int tabu_delta = 10000;
    int count = 0, tabu_count = 0;
    int A = best_conflict - conflict;
    int c_color;//当前结点颜色
    int* h_color;//邻接颜色表行首指针
    int* h_tabu;//禁忌表行首指针
    int c_color_table;//当前结点颜色表的值

    for (int i = 0; i < num_vertex; i++) 
    {//11.3
        c_color = solution[i];//6.1%
        h_color = adj_color_table[i];
        c_color_table = h_color[c_color];
        if (h_color[c_color] > 0) 
        {//17.6
            h_tabu = tabu_tenure[i];
            for (int j = 0; j < num_color; j++) 
            {
                if (c_color != j) 
                {//cpu流水线
                    //非禁忌移动
                    tmp = h_color[j] - c_color_table;
                    if (h_tabu[j] <= iter) 
                    {//22.6
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
        tmp = rand() % tabu_count;//相等delta随机选择
        node = equ_tabudelta[tmp][0];
        color = equ_tabudelta[tmp][1];
    }
    else 
    {
        tmp = rand() % count;//相等delta随机选择
        node = equ_delta[tmp][0];
        color = equ_delta[tmp][1];
    }
}

//class: 找最佳禁忌或者非禁忌移动
void Graph::find_move()
{
    delta = 10000;//初始为最大整数
    int tmp;//临时变量
    int tabu_delta = 10000;
    int count = 0, tabu_count = 0;
    int A = best_conflict - conflict;
    int c_color;//当前结点颜色
    int* h_color;//邻接颜色表行首指针
    int* h_tabu;//禁忌表行首指针
    int c_color_table;//当前结点颜色表的值

    for (int i = 0; i < num_vertex; i++)
    {//11.3
        c_color = solution[i];//6.1%
        h_color = adj_color_table[i];
        c_color_table = h_color[c_color];
        if (h_color[c_color] > 0)
        {//17.6
            h_tabu = tabu_tenure[i];
            for (int j = 0; j < num_color; j++)
            {
                if (c_color != j)
                {//cpu流水线
                    //非禁忌移动
                    tmp = h_color[j] - c_color_table;
                    if (h_tabu[j] <= iter)
                    {//22.6
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
        tmp = rand() % tabu_count;//相等delta随机选择
        node = equ_tabudelta[tmp][0];
        color = equ_tabudelta[tmp][1];
    }
    else
    {
        tmp = rand() % count;//相等delta随机选择
        node = equ_delta[tmp][0];
        color = equ_delta[tmp][1];
    }
}


//更新值
void make_move() 
{
    conflict = delta + conflict;//更新冲突值

    if (conflict < best_conflict) 
        best_conflict = conflict;//更新历史最好冲突

    int old_color = solution[node];
    solution[node] = color;
    tabu_tenure[node][old_color] = iter + conflict + rand() % 10 + 1;//更新禁忌表
    int* h_graph = adj_list[node];
    int num_edge = vertex_edge[node];
    int tmp;

    for (int i = 0; i < num_edge; i++) 
    {//更新邻接颜色表
        tmp = h_graph[i];
        adj_color_table[tmp][old_color]--;
        adj_color_table[tmp][color]++;
    }
}

// class: 更新值
void Graph::make_move()
{
    conflict = delta + conflict;//更新冲突值

    if (conflict < best_conflict)
        best_conflict = conflict;//更新历史最好冲突

    int old_color = solution[node];
    solution[node] = color;
    tabu_tenure[node][old_color] = iter + conflict + rand() % 10 + 1;//更新禁忌表
    int* h_graph = adj_list[node];
    int num_edge = vertex_edge[node];
    int tmp;

    for (int i = 0; i < num_edge; i++)
    {//更新邻接颜色表
        tmp = h_graph[i];
        adj_color_table[tmp][old_color]--;
        adj_color_table[tmp][color]++;
    }
}

// 禁忌搜索
void tabu_search(int seed, string path) 
{
    create_graph(path);
    // ofstream ofile("C:\\wamp64\\www\\npbenchmark\\npbenchmark-main\\SDK.GCP\\tabu_search\\total_O3.txt", ios::out);
    ofstream ofile("total_O3.txt", ios::out);
    double start_time, end_time;
    double elapsed_time;
 
    // srand(clock());
    srand(seed);

    initialization();
    start_time = clock();
    iter = 0;
    while (conflict > 0) 
    {
        iter++;
        // cout << "iter: " << iter << endl; 
        if ((iter % 100000) == 0) 
            ofile << iter << " " << conflict << " " << num_color << " " << delta << " " << best_conflict << endl;
        find_move();
        make_move();
    }

    end_time = clock();
    elapsed_time = (double(end_time - start_time)) / CLOCKS_PER_SEC;

    cout << "success, iterations: " << iter << " elapsed_time(s): " << elapsed_time << " frequency:" << double(iter / elapsed_time) << endl;

    // save solutions; 
    for (int i = 0;i < num_vertex; i++)
    {
        ofile << solution[i] << endl;
    }

    ofile.close();
}

// class: 禁忌搜索
void Graph::tabu_search(int seed, string path)
{
    create_graph(path);
    // ofstream ofile("C:\\wamp64\\www\\npbenchmark\\npbenchmark-main\\SDK.GCP\\tabu_search\\total_O3.txt", ios::out);
    ofstream ofile("total_O3.txt", ios::out);
    double start_time, end_time;
    double elapsed_time;

    // srand(clock());
    srand(seed);

    initialization();
    start_time = clock();
    iter = 0;
    while (conflict > 0)
    {
        iter++;
        // cout << "iter: " << iter << endl; 
        if ((iter % 100000) == 0)
            ofile << iter << " " << conflict << " " << num_color << " " << delta << " " << best_conflict << endl;
        find_move();
        make_move();
    }

    end_time = clock();
    elapsed_time = (double(end_time - start_time)) / CLOCKS_PER_SEC;

    cout << "success, iterations: " << iter << " elapsed_time(s): " << elapsed_time << " frequency:" << double(iter / elapsed_time) << endl;

    // save solutions; 
    for (int i = 0;i < num_vertex; i++)
    {
        ofile << solution[i] << endl;
    }

    ofile.close();
}


int main() 
{
    int seed = 2; 
     /*
    cout << "No class speed: " << endl;
    tabu_search(seed, "./data/DSJC0500.5.txt");
     */

    // /*
    Graph test; 
    cout << "In class speed: " << endl;
    test.tabu_search(seed, "./data/DSJC0500.5.txt");
    // */

    return 0;
}


