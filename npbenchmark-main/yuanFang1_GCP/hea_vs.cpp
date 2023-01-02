//
// Created by take_ on 2022/12/30.
//
// hea_vs.cpp : 定义控制台应用程序的入口点。
//

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iostream>
#include <fstream>

#define MaxPoint 1002
#define MaxColor 300
#define P 20

using namespace std;
int MaxIter = 16000;

struct Population_solution {
    int psol[MaxColor][MaxPoint];
    int num[MaxColor];
    int index1[MaxPoint], index2[MaxPoint];
};

struct Population {
    int min_conflict;
    int num_conflict[P + 1];
};

Population population;
Population_solution population_solution[P+1];
int conflict[MaxPoint], conflict_index[MaxPoint];
int conf_num = 0;

struct ArcNode {
    int adj_vertex;
    struct ArcNode *next;
};

struct VerNode {
    ArcNode *first;
};

struct Move {
    int u, vi, vj;
};

VerNode *adjList;
int **solution;
int adj_color_table[MaxPoint][MaxPoint], tabu_table[MaxPoint][MaxPoint];
int point_num, edge_num, f, best_conflict, k;
long long iter;
int res_iter;
double res_time;

void insert_adjList(int i, int j);
void dynamic_alloc();
int tabu_search(int *solution);
Move find_move(int *s);
void make_move(int u, int vj, int *s);

void add_conflict(int adj_vertex);
void delete_conflict(int adj_vertex);
void cross_over(int p1, int p2, int *index1);

/*
argv[1]:文件名
argv[2]:颜色数
*/
int main(int argc, char *argv[])
{
    int i, j, p;
    char c, s1[100], s2[100], file[100];
    FILE *fp;
    srand(6);

    sprintf(file, "./instances/%s", argv[1]);

    if ((fp = fopen(file, "r")) == nullptr)
    {
        cout << "file not open";
        return 0;
    }

    do
    {
        fgets(s1, 100, fp);
    } while (s1[0] == 'c');

    sscanf(s1, "%c %s %d %d", &c, s2, &point_num, &edge_num);

    dynamic_alloc();

    memset(adjList, 0, sizeof(VerNode)*(point_num + 1));

    while (!feof(fp))
    {
        fscanf(fp, "%c %d %d\n", &c, &i, &j);
        insert_adjList(i, j);
        insert_adjList(j, i);
    }

    population.min_conflict = 100000;
    k = atoi(argv[2]);

    for (p = 1; p <= P; p++)
    {
        memset(adj_color_table, 0, sizeof(adj_color_table));
        memset(tabu_table, 0, sizeof(tabu_table));
        f = best_conflict = conf_num = 0;

        for (i = 1; i <= point_num; i++)
        {
            solution[p][i] = rand() % k + 1;
            //cout << solution[i] <<' ';
        }

        tabu_search(solution[p]);
        population.num_conflict[p] = f;

        if (f < population.min_conflict)
        {
            population.min_conflict = f;
        }

        if (f == 0)
            break;
    }

    memset(population_solution, 0, sizeof(population_solution));
    clock_t start = clock();

    if (p > P)
    {
        for (p = 1; p <= P; p++)
        {
            for (i = 1; i <= point_num; i++)
            {
                int color = population_solution[p].index1[i] = solution[p][i];
                int color_num = population_solution[p].num[color];
                population_solution[p].psol[color][color_num] = i;
                population_solution[p].index2[i] = color_num++;
                population_solution[p].num[color] = color_num;
            }
        }

        Population_solution temps;
        int cnt = 0;
        while (population.min_conflict != 0)
        {
            int p1 = rand() % P + 1, p2;

            do
            {
                p2 = rand() % P + 1;
            } while (p1 == p2);

            memset(&temps, 0, sizeof(temps));

            cross_over(p1, p2, temps.index1);

            memset(adj_color_table, 0, sizeof(adj_color_table));
            memset(tabu_table, 0, sizeof(tabu_table));
            f = best_conflict = conf_num =0;

            tabu_search(temps.index1);//对temps进行禁忌搜索

            for (i = 1; i <= point_num; i++)
            {//变成划分的形式
                int color = temps.index1[i];
                int color_num = temps.num[color];
                temps.psol[color][color_num] = i;
                temps.index2[i] = color_num;
                temps.num[color] = ++color_num;
            }

            int max_conflict = -1, max_p;

            for (i = 1; i <= P; i++)
            {
                if (population.num_conflict[i] > max_conflict)
                {
                    max_conflict = population.num_conflict[i];
                    max_p = i;
                }
            }

            population_solution[max_p] = temps;//将种群中冲突数最大的替换成temps
            population.num_conflict[max_p] = f;

            if (f<population.min_conflict)
            {
                population.min_conflict = f;
            }

            cout << "min conflict = " << population.min_conflict << endl;
        }
    }

    clock_t ends = clock();
    res_time = (double)(ends - start) / CLOCKS_PER_SEC;
    cout << res_time<<endl;

    if (population.min_conflict == 0)
    {
        fp = fopen("result.txt", "a+");
        if (fp == nullptr)
            printf("output file open error\n");
        fprintf(fp, "%s %-9s %-15lf %-7d\n", argv[1], argv[2], res_time , MaxIter);
    }
    else
        cout << "over time" << endl;

    return 0;
}

void insert_adjList(int i, int j)
{
    ArcNode *temp1 = (ArcNode *)malloc(sizeof(ArcNode));
    temp1->adj_vertex = i;
    temp1->next = adjList[j].first;
    adjList[j].first = temp1;
}

void dynamic_alloc()
{
    adjList = new VerNode [point_num + 1];

    solution = new int *[P + 1];
    for (int i = 0; i <= P; i++)
        solution[i] = new int [point_num + 1];
}

int tabu_search(int *solution)
{
    bool is_conflict;
    //cout <<endl;

    for (int i = 1; i <= point_num; i++)
    {
        ArcNode *temp = adjList[i].first;
        is_conflict = false;
        while (temp)
        {
            if (solution[temp->adj_vertex] == solution[i])
            {
                is_conflict = true;
                f++;
            }
            adj_color_table[i][solution[temp->adj_vertex]]++;
            temp = temp->next;
        }

        if (is_conflict)
        {
            conflict[conf_num] = i;
            conflict_index[i] = conf_num++;
        }
    }

    f = f / 2;
    //cout <<"initial_f = "<<f<<endl;
    best_conflict = f;
    iter = 0;

    while (iter < MaxIter)
    {
        if (f == 0)
            break;
        Move my_move = find_move(solution);
        make_move(my_move.u, my_move.vj, solution);
        iter++;
    }

    if (iter == MaxIter)
        return 0;

    res_iter = iter;

    return 1;
}

Move find_move(int *s)
{
    int i, j, conf_i;
    int sol_i;
    Move tabu_move[MaxPoint], non_tabu_move[MaxPoint];
    int tabu_cnt = 1, non_tabu_cnt = 1;
    int tabu_move_delt = 100000, non_tabu_move_delt = 100000;

    for (conf_i = 0; conf_i< conf_num; conf_i++)
    {
        i = conflict[conf_i];
        sol_i = s[i];
        if (adj_color_table[i][sol_i]>0)
        {
            for (j = 1; j <= k; j++)
            {
                if (j != sol_i)
                {
                    int temp_delt = adj_color_table[i][j] - adj_color_table[i][sol_i];
                    //cout <<temp_delt<<'\t';
                    if (iter >= tabu_table[i][j])
                    {
                        if (temp_delt <= non_tabu_move_delt)
                        {
                            if (temp_delt < non_tabu_move_delt)
                            {
                                non_tabu_cnt = 0;
                                non_tabu_move_delt = temp_delt;
                            }
                            non_tabu_move[non_tabu_cnt].u = i;
                            non_tabu_move[non_tabu_cnt++].vj = j;
                        }
                    }
                    else
                    {
                        if (temp_delt <= tabu_move_delt)
                        {
                            if (temp_delt < tabu_move_delt)
                            {
                                tabu_cnt = 0;
                                tabu_move_delt = temp_delt;
                            }
                            tabu_move[tabu_cnt].u = i;
                            tabu_move[tabu_cnt++].vj = j;
                        }
                    }
                }
            }
        }
    }

    int temp1 = f + tabu_move_delt, temp2 = f + non_tabu_move_delt;
    Move res;
    if (temp1<best_conflict && temp1<temp2)
    {
        f = best_conflict = temp1;
        int index = rand() % tabu_cnt;
        res = tabu_move[index];
    }
    else
    {
        if (temp2 < best_conflict)
            best_conflict = temp2;
        f = temp2;
        int index = rand() % non_tabu_cnt;
        res = non_tabu_move[index];
    }
    return res;

}

void make_move(int u, int vj, int *s)
{
    int vi = s[u];
    s[u] = vj;
    tabu_table[u][vi] = f + iter + rand() % 10 + 1;
    ArcNode *temp = adjList[u].first;

    while (temp)
    {
        int adj_vertex = temp->adj_vertex;
        if ((--adj_color_table[adj_vertex][vi]) == 0)
        {
            if (s[adj_vertex] == vi)
            {
                delete_conflict(adj_vertex);
            }
        }

        if ((++adj_color_table[adj_vertex][vj]) == 1)
        {
            if (s[adj_vertex] == vj)
            {
                add_conflict(adj_vertex);
            }
        }
        temp = temp->next;
    }

    if (adj_color_table[u][vi] != 0 && adj_color_table[u][vj] == 0)
        delete_conflict(u);
    if (adj_color_table[u][vi] == 0 && adj_color_table[u][vj] != 0)
        add_conflict(u);
}

void add_conflict(int adj_vertex)
{
    conflict[conf_num] = adj_vertex;
    conflict_index[adj_vertex] = conf_num++;
}

void delete_conflict(int adj_vertex)
{
    int temp_index = conflict_index[adj_vertex];
    conflict[temp_index] = conflict[--conf_num];
    conflict_index[conflict[temp_index]] = temp_index;
}


void cross_over(int p1, int p2, int *index1)
{
    int A, B;
    Population_solution s[2];
    s[0] = population_solution[p1];
    s[1] = population_solution[p2];

    for (int i = 1; i <= k; i++)
    {
        if (i % 2 != 0)
        {
            A = 0;
            B = 1;
        }
        else
        {
            A = 1;
            B = 0;
        }

        int max_index, max_num = -1, *h_num = s[A].num;
        for (int j = 1; j <= k; j++)
        {
            if (h_num[j] >max_num)
            {
                max_index = j;
                max_num = h_num[j];
            }
        }

        int num = h_num[max_index];
        int *h_color = s[A].psol[max_index];
        for (int j = 0; j<num; j++)
        {
            int point = h_color[j];
            index1[point] = i;//只需要保存哪个点分配了哪种颜色，因为马上要对它进行禁忌搜索，其它的保存了又会变

            int color = s[B].index1[point];//在B中删除这个点
            int index2 = s[B].index2[point];
            int t = s[B].psol[color][index2] = s[B].psol[color][--s[B].num[color]];
            s[B].index2[t] = index2;
        }

        //删除这些点
        s[A].num[max_index] = 0;

    }

    for (int i = 1; i <= k; i++)
    {
        int num = s[0].num[i];
        for (int j = 0; j<num; j++)
        {
            int point = s[0].psol[i][j];
            int color = rand() % k + 1;//随机分配到某一种颜色中去
            index1[point] = color;
        }
    }
}