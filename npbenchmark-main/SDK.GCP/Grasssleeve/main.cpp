// Tabu08.cpp: 定义控制台应用程序的入口点。
//


#include<iostream>
#include<time.h>
#include<string>
#include<fstream>
#include<stdlib.h>

using namespace std;

int N, K;
int* sol; //保存每个点的颜色
int** adj_color_table; //邻域颜色表,禁忌表
int** tabutenure;
int f, best_f;//冲突数
int **NbID; //顶点数*(顶点数-1)，保存邻点有哪些
int* num_adj; //保存邻点个数
bool** Adj;  //相邻则为true
int** equ_delt;//保存相等的解


void Initialize(string fileName)
{
	//定义顶点是否相连的数组

	ifstream ifs;
	ifs.open(fileName);
	//char strRead[100];
	string str;
	ifs >> str;
	//ifs >> strRead;
	while (!ifs.eof())
	{
		if (str == "edge")
		{
			ifs >> N;
			////v = new Vertex[nVertex]; //初始化nV个顶点

			Adj = new bool*[N];
			for (int i = 0; i < N; i++)
			{
				Adj[i] = new bool[N];
			}
			for (int i = 0; i < N; i++)
			{
				for (int j = 0; j < N; j++)
					Adj[i][j] = false;
			}

		}
		if (str == "e")
		{
			int m, n;
			//ifs >> n >> vNext;
			//v[n].adjV.push_back(vNext);
			ifs >> m >> n;
			Adj[m - 1][n - 1] = true;
			Adj[n - 1][m - 1] = true;
		}
		ifs >> str;
	}
	ifs.close();

	//开辟equ_delt,最多有N*(K-1)个解.即每个顶点可以产生K-1个delt
	//.equ_delt[i][0]存第i个解的顶点，equ_delt[i][2]存第i个解的newcolor
	equ_delt = new int*[N*(K - 1)];
	for (int i = 0; i < N*(K - 1); i++) equ_delt[i] = new int[2];

	sol = new int[N];//每个顶点的颜色
	srand(time(NULL));
	for (int i = 0; i < N; i++)
	{
		sol[i] = rand() % K;  //给每个点随机着色
	}

	///////////开辟adjcolortable数组
	adj_color_table = new int*[N];
	for (int i = 0; i < N; i++)
	{
		adj_color_table[i] = new int[K];
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < K; j++) adj_color_table[i][j] = 0;
	}
	//初始化adj_color_table
	for (int i = 0; i < N; i++)
	{
		for (int j = i + 1; j < N; j++)
		{
			if (Adj[i][j])  //点i和点j相邻
			{
				adj_color_table[i][sol[j]]++;
				adj_color_table[j][sol[i]]++;
				if (sol[i] == sol[j])
				{
					f++;  //两点颜色相等，则冲突数加一
				}
			}
		}
	}
	best_f = f;  //初始化最优f

	//初始化tabutenure
	tabutenure = new int*[N];
	for (int i = 0; i < N; i++)
	{
		tabutenure[i] = new int[K];
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < K; j++) tabutenure[i][j] = 0;
	}
	//初始化NbID为N*(N-1)数组和num_adj
	num_adj = new int[N];
	for (int i = 0; i < N; i++) num_adj[i] = 0;
	NbID = new int*[N];
	for (int i = 0; i < N; i++) NbID[i] = new int[N - 1];
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N - 1; j++) NbID[i][j] = 0;
	}
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			if (Adj[i][j]) {
				int tmp = num_adj[i];
				num_adj[i]++;
				NbID[i][tmp] = j;  //等于去掉了adj表中的非零元素
			}
		}
	for (int i = 0; i != N; i++)
		delete[] Adj[i];
	delete[] Adj;
}

int delt;//此次迭代的最优解
int equ_count = 0;//记录相等解的个数
int iter;//迭代次数
int sel_vertex, sel_color;// move

void findmove() {
	delt = 10000;//初始为最大整数
	int tmp_delt;
	int c_color;//当前结点颜色
	int *h_color;//邻接颜色表行首指针
	int *h_tabu;//禁忌表行首指针
	int c_color_table;//当前结点邻接颜色表的值
	for (int i = 0; i < N; i++) {
		c_color = sol[i];
		h_color = adj_color_table[i];
		c_color_table = h_color[c_color];//即adj_color_table[i][sol[i]]的值
		if (c_color_table > 0) {  //颜色表此处的值不为0
			h_tabu = tabutenure[i];
			for (int j = 0; j < K; j++) {
				if (c_color != j) { //如果颜色不相同
					tmp_delt = h_color[j] - c_color_table;
					if (tmp_delt <= delt && (iter > h_tabu[j] || (tmp_delt + f) < best_f)) { //应该把更常出现的情况写在前面,实测差别不大
/*
						if (tmp_delt < delt) {
							equ_count = 1;
							delt = tmp_delt;
							sel_vertex = i;
							sel_color = j;
						}
						int time_count = 100000;
						clock_t start, end;
						start = clock();
						while (time_count) {
							if (rand() % equ_count == 0) {
								sel_vertex = i;
								sel_color = j;
							}
							time_count--;
						}
						end = clock();
						cout << "取余时间为：" << end - start << endl;
							equ_count++;
							*/

						if (tmp_delt < delt) {//当前解小于本次迭代最优解,则重新开始保存解

							equ_count = 0;
							delt = tmp_delt;

						}
						equ_delt[equ_count][0] = i;
						equ_delt[equ_count][1] = j;
						equ_count++;//end of another way
					}
				}
			}
		}
	}

	int tmp = rand() % equ_count;//有多个解时，随机选择
	sel_vertex = equ_delt[tmp][0];
	sel_color = equ_delt[tmp][1];
}
void makemove() {
	f = delt + f;
	if (f < best_f) best_f = f;
	int old_color = sol[sel_vertex];
	sol[sel_vertex] = sel_color; //更新颜色
	tabutenure[sel_vertex][old_color] = iter + f + 0.6*(rand() % 10);//更新禁忌表，是否最后要+1？？
	//还要有数组存下每个顶点的邻边，以及邻边的数量
	int* h_NbID = NbID[sel_vertex];
	int num_edge = num_adj[sel_vertex];
	int tmp;
	for (int i = 0; i < num_edge; i++) {//更新邻接颜色表
		tmp = h_NbID[i];//等于nbID[sel_vertex][i]
		adj_color_table[tmp][old_color]--;
		adj_color_table[tmp][sel_color]++;
	}
	//if(f==1)
	//	cout<<"Vertex="<<sel_vertex << "\t oldcolor=" << old_color << "\t newcolor=" << sel_color <<"\t iter="<<iter<<"\t tt="<< tabutenure[sel_vertex][old_color]<< endl;
}



int main()
{
	int result_count = 10;//自动计算10次
	while (result_count) {
		K = 49;
		Initialize("DSJC500.5.col");
		iter = 0;
		clock_t start, end;
		start = clock();
		while (f)
		{
			//if (f == 1)
			//	int i = 0;
			findmove();
			makemove();
			iter++;
		}
		end = clock();
		cout << "iter=" << iter << "\t time=" << (end - start) << endl;
		for (int i = 0; i < N; i++)
			delete[]adj_color_table[i];
		delete[]adj_color_table;

		delete[]sol;

		for (int i = 0; i < N; i++)
			delete[]tabutenure[i];
		delete[]tabutenure;

		for (int i = 0; i < N; i++)
			delete[]NbID[i];
		delete[]NbID;

		delete[]num_adj;

		for (int i = 0; i < K - 1; i++)
			delete[]equ_delt[i];
		delete[]equ_delt;

		result_count--;
	}
	system("pause");
	return 0;
}