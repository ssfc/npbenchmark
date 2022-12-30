//
// Created by take_ on 2022/12/30.
//

// HEA02.cpp: 定义控制台应用程序的入口点。
//

#include<cstring>
#include<string>
#include<iostream>
#include<fstream>
#include<time.h>
#include<map>

using namespace std;
#define POPULATION_NUM 10  //种群数据设为10
#define MAX_TABU_ITER 10000//tabu search最多的迭代次数
#define SOL POPULATION_NUM+1  //存储解

//#define SHOWBUG

int N, K;
int **NbID; //顶点数*(顶点数-1)，保存邻点有哪些
int* num_adj; //保存邻点个数
int** sol;//保存解 [第i个解][第j个顶点] 即p*N
int** adj_table;//邻接颜色表
int** tabutenure;

int* color_num;//保存每种颜色的个数，大小为K

int* s;
int f_p;//当前种群的f
void Tabu(int p);
void Initialize_Popu(); //初始化种群
void Initialloc();//开辟数组

void Initialloc()
{
    //开辟邻接颜色表
    adj_table = new int*[N];
    for (int i = 0; i < N; i++)
        adj_table[i] = new int[K];
    /////////////////////////////////////
    ///////////开辟tt
    tabutenure = new int*[N];
    for (int i = 0; i < N; i++)
        tabutenure[i] = new int[K];
    /////开辟color_num
    color_num = new int[K];


}

void Deleteloc()
{
    for (int i = 0; i < N; i++)
        delete[]NbID[i];
    delete[]NbID;

    delete[]num_adj;

    for (int i = 0; i < N; i++)
        delete[]adj_table[i];
    delete[]adj_table;

    for (int i = 0; i < POPULATION_NUM + 1; i++)
        delete[]sol[i];
    delete[]sol;

    for (int i = 0; i < N; i++)
        delete[]tabutenure[i];
    delete[]tabutenure;

    delete[]color_num;
    //delete[]s;
}


void Initialize(string fileName)
{
    ifstream ifs;
    ifs.open(fileName);
    string str;
    int tmp;
    ifs >> str;
    while (!ifs.eof())
    {
        if (str == "edge")
        {
            ifs >> N;
            //初始化NbID为N*(N-1)数组和num_adj
            num_adj = new int[N];
            for (int i = 0; i < N; i++) num_adj[i] = 0;
            NbID = new int*[N];
            for (int i = 0; i < N; i++) NbID[i] = new int[N - 1];
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N - 1; j++) NbID[i][j] = 0;
            }
        }
        if (str == "e")
        {
            int m, n;  //顶点的ID
            ifs >> m >> n;
            m--;
            n--;
            //保存邻边信息
            tmp = num_adj[m];
            NbID[m][tmp] = n;
            num_adj[m]++;

            tmp = num_adj[n];
            NbID[n][tmp] = m;
            num_adj[n]++;
        }
        ifs >> str;
    }
    ifs.close();
    srand((unsigned int)time(NULL));
    Initialize_Popu();

    Initialloc();


    for (int i = 0; i < POPULATION_NUM; i++)
    {
        Tabu(i);
        if (!f_p) break;//重要
    }
}

void Initialize_Popu() //初始化种群
{
    int* h_sol;
    sol = new int*[POPULATION_NUM + 1];
    for (size_t i = 0; i < POPULATION_NUM + 1; i++)
    {
        sol[i] = new int[N];
    }
    for (int i = 0; i < POPULATION_NUM; i++)
    {
        h_sol = sol[i];
        for (int j = 0; j < N; j++)
            h_sol[j] = rand() % K;
    }
}



////禁忌算法

int best_f = 10000;
int f[SOL] = { 10000 };
void Initialize_Adj()//计算邻接颜色表
{
    //int* s = sol[p];  //第p个种群
    for (int i = 0; i < N; i++)
        for (int j = 0; j < K; j++)
            adj_table[i][j] = 0;

    int* h_adj_table;
    int* h_NbID;
    int edge_v;//邻点
    int edge_v_color;//邻点颜色
    int c_color;//当前顶点的颜色

    f_p = 0;
    for (int i = 0; i < N; i++)
    {
        h_adj_table = adj_table[i];
        h_NbID = NbID[i];
        c_color = s[i];
        for (int j = 0; j < num_adj[i]; j++)
        {
            edge_v = h_NbID[j];//得到邻点ID、
            edge_v_color = s[edge_v];//得到邻点颜色
            h_adj_table[edge_v_color]++;
            if (c_color == edge_v_color) f_p++;
        }
    }
    f_p = f_p / 2;
    best_f = f_p;
#ifdef SHOWBUG
    cout << "f_Initial:" << f_p << endl;
#endif
}

void Initialize_TT()//禁忌表
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < K; j++)
            tabutenure[i][j] = 0;
}

int delt;
int iter;
int equ_count;
int equ_delt[2000][2];//先试试写死
int sel_vertex, sel_color;

void findmove()
{
    delt = 10000;//初始为最大整数
    int tmp_delt;
    int c_color;//当前结点颜色
    int *h_color;//邻接颜色表行首指针
    int *h_tabu;//禁忌表行首指针
    int c_color_table;//当前结点邻接颜色表的值
    for (int i = 0; i < N; i++) {
        c_color = s[i];
        h_color = adj_table[i];
        c_color_table = h_color[c_color];//即adj_color_table[i][sol[i]]的值
        if (c_color_table > 0) {  //颜色表此处的值不为0
            h_tabu = tabutenure[i];
            for (int j = 0; j < K; j++) {
                if (c_color != j) { //如果颜色不相同
                    tmp_delt = h_color[j] - c_color_table;
                    if (tmp_delt <= delt && (iter > h_tabu[j] || (tmp_delt + f_p) < best_f)) {
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
void makemove()
{
    f_p = delt + f_p;
    if (f_p < best_f) best_f = f_p;
    int old_color = s[sel_vertex];
    s[sel_vertex] = sel_color; //更新颜色
    tabutenure[sel_vertex][old_color] = iter + f_p + (rand() % 10) + 1;//更新禁忌表，是否最后要+1？？
    //还要有数组存下每个顶点的邻边，以及邻边的数量
    int* h_NbID = NbID[sel_vertex];
    int num_edge = num_adj[sel_vertex];
    int tmp;
    for (int i = 0; i < num_edge; i++) {//更新邻接颜色表
        tmp = h_NbID[i];//等于nbID[sel_vertex][i]
        adj_table[tmp][old_color]--;
        adj_table[tmp][sel_color]++;
    }
}


void Tabu(int p) //输入的是第p个种群
{
    s = sol[p];//指向当前种群
    f_p = f[p];//赋值当前种群的f，最后再赋值回去
    Initialize_Adj();
    Initialize_TT();
    iter = 0;
    while (iter < MAX_TABU_ITER && best_f)
    {
        findmove();
        makemove();
        iter++;
    }
    f[p] = f_p;//感觉不太对，此时s不一定是最小f时的s了，但是best_f时的解，无法保存吧
#ifdef SHOWBUG
    cout << "f[" << p << "]:" << f_p << endl;
	cout << endl;
#endif
}


//#define SHOWNUM
map<int, int> s1, s2, *h_s;
map<int, int>::iterator it, itTmp;
int max_equ_count, max_k[200];

void CrossOver()
{
    max_equ_count = 0;
    int count_s = 0;
    memset(color_num, 0, K * sizeof(int));

#ifdef SHOWNUM
    for (int i = 0; i < K; i++)
	{
		cout << "num of " << i << " is " << color_num[i] << endl;
	}
#endif // SHOWNUM
    int m, n;
    m = rand() % POPULATION_NUM;
    while ((n = rand() % POPULATION_NUM) == m); //避免选了两个一样的种群
#ifdef SHOWBUG
    cout << "s1=" << m << "\t s2=" << n << endl;
#endif // SHOWBUG



    int* h_s1 = sol[m], *h_s2 = sol[n];
    for (int i = 0; i < N; i++)
    {
        s1[i] = h_s1[i];
        s2[i] = h_s2[i];
    }

    bool odd = false;
    int c_color, c_v;
    for (int l = 0; l < K; l++)
    {
        memset(color_num, 0, K * sizeof(int));//每次都要清零
        if (odd)
        {
            odd = false;
            h_s = &s1;
        }
        else
        {
            odd = true;
            h_s = &s2;
        }

        it = h_s->begin();
        while (it != h_s->end())
        {
            c_color = it->second;
            color_num[c_color]++;
            it++;
        }
        int tmp_max = -1;

#ifdef SHOWNUM
        for (int i = 0; i < K; i++)
		{
			cout << "num of " << i << " is " << color_num[i] << endl;
		}

		int total = 0;
		for (int i = 0; i < K; i++)
		{
			total += color_num[i];
		}
		cout << "total num is " << total << endl;
#endif // SHOWNUM

        for (int i = 0; i < K; i++)
        {
            if (tmp_max <= color_num[i])
            {
                if (tmp_max < color_num[i])
                {
                    tmp_max = color_num[i];//保存下所有的最大card的颜色然后随机选一个

                    max_equ_count = 0;
                }
                max_k[max_equ_count] = i;
                max_equ_count++;
            }
        }
        int max_card;//最大种群的颜色
        max_card = max_k[rand() % max_equ_count];//从最大颜色中随机选了一个


        it = h_s->begin();
        while (it != h_s->end())
        {

            c_color = it->second;
            if (c_color == max_card)
            {
                c_v = it->first;
                it++;

                s1.erase(c_v);
                s2.erase(c_v);
                sol[SOL - 1][c_v] = l;
            }
            else it++;
        }

    }

    it = s1.begin();
    while (it != s1.end())
    {
        c_v = it->first;
        sol[SOL - 1][c_v] = rand() % K;//随机设
        it++;
    }

}

#define SHOWRESULT
int main()
{
    int count = 10;

    while (count)
    {
        bool flag_update = true;//种群是否更新过
        int best_p;
        int min_f = 10000;
        K = 48;
        Initialize("DSJC500.5.col");
        for (int i = 0; i < POPULATION_NUM; i++)
        {
            if (min_f > f[i])
            {
                min_f = f[i];
                best_p = i;
            }
        }
        int max_crossover_num = 2000;
        int equ_worst_f[2000], worst_f_count;

        clock_t start, end;
        start = clock();
        while (max_crossover_num && min_f)
        {
            CrossOver();
            Tabu(SOL - 1);
            if (f[SOL - 1] < min_f) //s0<S*
            {
                //best_p = SOL - 1;
                min_f = f[SOL - 1];
            }
            int tmp_worst = -1;
            int worst_p;//最差的种群
//////////////////新策略
/*			if (flag_update)
			{
				for (int i = 0; i < POPULATION_NUM; i++) //找出最差解
				{
					if (tmp_worst <= f[i])
					{
						if (tmp_worst < f[i])
						{
							tmp_worst = f[i];
							worst_f_count = 0;
						}
						equ_worst_f[worst_f_count] = i;
						worst_f_count++;
					}
				}
				worst_p = equ_worst_f[rand() % worst_f_count];
			}
			//pool update
			if (f[worst_p] > f[SOL - 1])//当offspring s的冲突数比population里面最大的冲突数的解sw要好，就将sw替换为s，否则以0.3的概率将sw替换为s
			{
				memcpy(sol[worst_p], sol[SOL - 1], N * sizeof(int));
				f[worst_p] = f[SOL - 1];
				flag_update = true;
			}
			else if(rand() % 100 < 30) //两句不能用或写在一起，因为是1不满足时，才看2是否满足？？好像不对
			{
				memcpy(sol[worst_p], sol[SOL - 1], N * sizeof(int));
				f[worst_p] = f[SOL - 1];
				flag_update = true;
			}else flag_update = false;//没有更新过
*/
///////////////新策略end
            //////原策略

            for (int i = 0; i < POPULATION_NUM; i++) //找出最差解
            {
                if (tmp_worst <= f[i])
                {
                    if (tmp_worst < f[i])
                    {
                        tmp_worst = f[i];
                        worst_f_count = 0;
                    }
                    equ_worst_f[worst_f_count] = i;
                    worst_f_count++;
                }
            }
            worst_p = equ_worst_f[rand() % worst_f_count];

            memcpy(sol[worst_p], sol[SOL - 1], N * sizeof(int));
            f[worst_p] = f[SOL - 1];
            ////////////////////原策略end
            max_crossover_num--;
        }
        end = clock();


#ifdef SHOWRESULT
        //cout << "best solution is population " << best_p << endl;
        cout << "NO." << 10 - count << "\t K=" << K << "\t Cross Iter= " << 2000 - max_crossover_num << "\t time=" << (end - start) << endl;
#endif // SHOWRESULT

        count--;
        Deleteloc();
    }



    //cout << "min_f=" << min_f << endl;
    return 0;
}
