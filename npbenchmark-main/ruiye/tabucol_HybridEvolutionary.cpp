//
// Created by take_ on 2022/12/31.
//
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>

#define POP 10
#define INF 100000

using namespace std;
ifstream fin("DSJC500.5.txt");
ofstream fout("output.txt");

int Final_Colornum;//最终颜色数
int Colornum; //颜色总数
int Iter1, Iter2, Iter3;
int *Conflictnum;
int Unsuccess1, Unsuccess2;
int Conflict, Conflict_best;//最佳冲突边数
int N, E; // 节点数和边数
int **Graph; //二维数组存储图
int **Adjac; //邻接表
int *Adjaclen; //邻接表长度
int **Color; //节点的当前颜色
int **Opt; //节点颜色变化的优化表
int **Tabu; //禁忌表
int *NewColor;//杂交产生的新子代
int *BestColor;

//初始化 无向图（二维数组存储 顶点数决定有多大，边记为1）
//初始化 邻接表（二维数组存储相邻顶点号 一维数组存储相邻节点个数 通过边信息产生）
void initialization()
{
    char ch;
    int i, j;
//	cout << "请输入顶点数 边数：" << endl;
    //cin >> N >> E;
    fin >> N >> E;
    Graph = new int *[N + 2];
    Adjac = new int *[N + 2];
    Adjaclen = new int[N + 2];
    Color = new int *[POP + 2];
    Conflictnum = new int[POP + 2];//函数
    Opt = new int *[N + 2];
    Tabu = new int *[N + 2];
    NewColor = new int[N + 2];
    BestColor = new int[N + 2];
    for (i = 0; i < POP; ++i)
        Color[i] = new int[N + 2];
    for (i = 0; i < N; i++)
    {
        Graph[i] = new int[N + 2];
        Adjac[i] = new int[N + 2];
        Opt[i] = new int[Colornum + 2];
        Tabu[i] = new int[Colornum + 2];
        Adjaclen[i] = 0;
    }

    //二维数组存储 邻接表 初始化为0
    for (i = 0; i < N; ++i)
        for (j = 0; j < N; ++j)
        {
            Graph[i][j] = 0;
            Adjac[i][j] = 0;
        }
    //创建邻接表 和 图形表
    for (i = 1; i <= E; ++i)
    {
        int a, b; //有关系的两个点
//		cout << "请输入：字符 有冲突两点的顶点号" << endl;
        //cin >> ch >> a >> b;
        fin >> ch >> a >> b;
        a--;
        b--;
        Graph[a][b] = Graph[b][a] = 1;
        Adjac[a][Adjaclen[a]] = b;
        Adjaclen[a]++;
        Adjac[b][Adjaclen[b]] = a;
        Adjaclen[b]++;
    }

}

//初始化着色方案 接着计算初始方案的冲突情况
void Prepare()
{
    //节点随机着色
    //POP是多种方案同时进行
    for (int i = 0; i < POP; ++i)
        for (int j = 0; j < N; ++j)
            Color[i][j] = rand() % Colornum;
    //计算目标函数：初始着色方案的冲突度 （相邻且同色则加1）
    for (int l = 0; l < POP; ++l)
    {
        Conflictnum[l] = 0;
        for (int i = 0; i < N - 1; ++i)
            for (int j = i + 1; j < N; ++j)
                if (Graph[i][j] == 1 &&Color[l][i] == Color[l][j])
                    Conflictnum[l]++;
    }
}

//根据10个方案产生新方案
void Crossover()
{
    int ColorsumA[200], ColorsumB[200];
    int Sign, SN = 0;
    int Coloring = 0;//当前颜色
    int A, B;
    int MaxA, MaxB;
    int SignA[60], SignB[60];
    int SignAlen, SignBlen;
    int i;

    for (i = 0; i < Colornum; ++i)
    {
        ColorsumA[i] = 0;
        ColorsumB[i] = 0;
    }
    for (i = 0; i < N; ++i)
        NewColor[i] = -1;
    //多个方案挑选两个不一样的
    while (1)
    {
        A = rand() % POP;
        B = rand() % POP;
        if (A != B)
            break;
    }
    for (i = 0; i < N; ++i)
    {
        ColorsumA[Color[A][i]]++;
        ColorsumB[Color[B][i]]++;
    }
    while (Coloring < Colornum && SN < N)
    {
        MaxA = MaxB = -INF;
        for (int i = 0; i<Colornum; ++i)
        {
            if (ColorsumA[i]>MaxA)
            {
                MaxA = ColorsumA[i];
                SignAlen = 0;
                SignA[0] = i;
                SignAlen++;
            }
            else if (ColorsumA[i] == MaxA && SignAlen <= 50)
            {
                SignA[SignAlen] = i;
                SignAlen++;
            }
            if (ColorsumB[i]>MaxB)
            {
                MaxB = ColorsumB[i];
                SignBlen = 0;
                SignB[0] = i;
                SignBlen++;
            }
            else if (ColorsumB[i] == MaxB && SignBlen <= 50)
            {
                SignB[SignBlen] = i;
                SignBlen++;
            }
        }
        if (Coloring % 2 == 0)
        {
            SN += MaxA;
            int k = rand() % SignAlen;
            Sign = SignA[k];
            ColorsumA[Sign] = -1;
            for (int i = 0; i<N; ++i)
                if (NewColor[i] == -1 && Color[A][i] == Sign)
                {
                    NewColor[i] = Coloring;
                    ColorsumB[Color[B][i]]--;
                }
        }
        else
        {
            SN += MaxB;
            int k = rand() % SignBlen;
            Sign = SignB[k];
            ColorsumB[Sign] = -1;
            for (int i = 0; i<N; ++i)
                if (NewColor[i] == -1 && Color[B][i] == Sign)
                {
                    NewColor[i] = Coloring;
                    ColorsumA[Color[A][i]]--;
                }
        }
        Coloring++;

    }
    Unsuccess1 = N - SN;
    Unsuccess2 = 0;
    for (i = 0; i<N; ++i)
        if (NewColor[i] == -1)
        {
            NewColor[i] = Color[A][i];
            Unsuccess2++;
        }
}

//初始化OPT表 和 Tabu表
void Clear_And_Prepare()
{
    Conflict = 0;
    int i, j;
    for (i = 0; i<N; ++i)
        for (j = 0; j<Colornum; ++j)
            Opt[i][j] = 0, Tabu[i][j] = 0;
    for (i = 0; i<N - 1; ++i)
        for (j = i + 1; j<N; ++j)
            if (Graph[i][j] == 1)
            {
                Opt[i][NewColor[j]]++;
                Opt[j][NewColor[i]]++;
                if (NewColor[i] == NewColor[j]) Conflict++;
            }
    for (i = 0; i<N; ++i)
        BestColor[i] = NewColor[i];
    Conflict_best = Conflict;
}

/*local search*/
void Tabu_Search()
{
    int temp,k;
    int p,q;
    int delta;
    int SP_delta;
    int Vertex[60],Col[60],len;
    int SP_Vertex[60],SP_Col[60],SP_len;
    int Iter;
    int i;
    len=0;
    SP_len=0;
    Iter3=0;
    Iter=0;
    while (Iter<500000)
    {
        Iter3++;
        delta=SP_delta=INF;
        for (int i=0;i<N;++i)
            // find one move
            if (Opt[i][NewColor[i]]!=0) //找到所有冲突的节点 颜色
            {
                //对同一个节点 找到增量最小的点
                for (int j=0;j<Colornum;++j)
                    if (NewColor[i]!=j) //换成另一个颜色
                    {
                        temp=Opt[i][j]-Opt[i][NewColor[i]];  //增量
                        if (Tabu[i][j]<=Iter3) //不禁忌
                        {
                            if (temp<delta)
                            {
                                delta=temp;len=0;
                                Vertex[len]=i; //记录改动的点
                                Col[len]=j;    //记录改后的颜色
                                len++;
                            }
                            else if (temp==delta && len<=50) //多个相同的值 只记录50个
                            {
                                Vertex[len]=i;
                                Col[len]=j;
                                len++;
                            }
                        }
                        else              //禁忌
                        {
                            if (temp<SP_delta)
                            {
                                SP_delta=temp;SP_len=0;
                                SP_Vertex[SP_len]=i;
                                SP_Col[SP_len]=j;
                                SP_len++;
                            }
                            else if (temp==SP_delta && SP_len<=50)
                            {
                                SP_Vertex[SP_len]=i;
                                SP_Col[SP_len]=j;
                                SP_len++;
                            }
                        }
                    }
            }
        //可能不符合禁忌表的情况
        if (SP_len>0 && SP_delta<delta && SP_delta+Conflict<Conflict_best)
        {
            k=rand()%SP_len;
            p=SP_Vertex[k];q=SP_Col[k];
            Conflict=SP_delta+Conflict;
        }
        else if (len>0)
        {
            k=rand()%len;
            p=Vertex[k];q=Col[k]; //one move
            Conflict=delta+Conflict;
        }
        else return;
        //更新改进后的结果
        if (Conflict<Conflict_best)
        {
            Conflict_best=Conflict;
            Iter=0;
            for (i=0;i<N;++i)
                BestColor[i]=NewColor[i];
        }
        else Iter++;
        if (Conflict_best==0)  //不需要改了 已经找到一个最优解
        {
            Colornum--;
            return;
        }
        //修改opt表，新一轮尝试
        for (i=0;i<Adjaclen[p];i++)
        {
            int t=Adjac[p][i];
            Opt[t][NewColor[p]]--;//修改优化表 变前同色冲突 都减1
            Opt[t][q]++; //变后同色冲突都加1
        }
        Tabu[p][NewColor[p]]=Iter3+rand()%10+1+Conflict; //修改禁忌表
        NewColor[p]=q;//修改颜色
    }
}

void Updating_POP()
{
    int Maxm = -INF;
    int *Pos, Poslen, P;
    Pos = new int[20];
    for (int i = 0; i<POP; ++i)
        if (Conflictnum[i]>Maxm)
        {
            Maxm = Conflictnum[i];
            Poslen = 0;
            Pos[0] = i;
            Poslen++;
        }
        else if (Conflictnum[i] == Maxm)
        {
            Pos[Poslen] = i;
            Poslen++;
        }
    int k = rand() % Poslen;
    P = Pos[k];
    //随机选一个方案 使用新方案更新
    if (Conflict_best <= Maxm)
    {
        for (int i = 0; i<N; ++i)
            Color[P][i] = BestColor[i];
        Conflictnum[P] = Conflict_best;
    }
}

int main()
{
    int i;
    time_t start, stop;
    srand((unsigned)time(NULL));
    Final_Colornum = Colornum = 49;
    initialization();
    Iter1 = 0;
    while (Iter1 < 100000)
    {
        start = time(NULL);
        Iter1++;
        Prepare(); //一次初始方案
        Iter2 = 0;
        while (Iter2 < 1000000)
        {
            Crossover();
            Iter2++;
            Clear_And_Prepare();
            Tabu_Search();
            Updating_POP();
            if (Colornum < Final_Colornum)
            {
                Final_Colornum = Colornum;
                break;
            }
            cout << "第" << Iter2 << "次尝试颜色数为：" << Colornum << endl;
            fout << "第" << Iter2 << "次尝试颜色数为：" << Colornum << endl;
            for (int i = 0; i < POP; ++i)
                cout << Conflictnum[i] << " ";
            cout << endl;
            for (i = 0; i < POP; ++i)
                fout << Conflictnum[i] << " ";
            fout << endl;
            //	cout << "子类覆盖失败" << Unsuccess1 << " " << Unsuccess2 << endl;
            //	fout << "子类覆盖失败" << Unsuccess1 << " " << Unsuccess2 << endl;
        }
        stop = time(NULL);
        cout << "使用时间为" << (stop - start) << "s" << endl;
        cout << "当前颜色为" << Final_Colornum + 1 << endl;
        cout << "最佳冲突边数为" << Conflict_best << endl;
        cout << "***********************************************" << endl;
        fout << "使用时间为" << (stop - start) << "s" << endl;
        fout << "当前颜色为" << Final_Colornum + 1 << endl;
        fout << "最佳冲突边数为" << Conflict_best << endl;
        fout << "***********************************************" << endl;
        if (Final_Colornum == 1)
            break;
    }
    cout << "最少颜色数为" << Final_Colornum + 1 << endl;
    return 0;
}


