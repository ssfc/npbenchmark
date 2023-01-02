//
// Created by take_ on 2022/12/30.
//
// hea_vs.cpp : 定义控制台应用程序的入口点。
//

#include <climits>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iostream>
#include <fstream>

#define MaxPoint 1002
#define MaxColor 300
#define num_population 20

using namespace std;
int max_iter = 16000;

struct Population_solution {
    int psol[MaxColor][MaxPoint];
    int color_num[MaxColor];
    int index1[MaxPoint], index2[MaxPoint];
};

struct Population {
    int min_conflict; // the min conflict among the population collection;
    int min_conflict_index;
    int num_conflict[num_population];
};

Population population;
Population_solution population_solution[num_population];
int conflict[MaxPoint], conflict_index[MaxPoint];
int conflict_num = 0;

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

VerNode *adj_list;
int **solution_collection;
int adj_color_table[MaxPoint][MaxPoint], tabu_tenure_table[MaxPoint][MaxPoint];
int num_vertex, num_edge, f, best_conflict, num_color;
long long iter;
int res_iter;
double elapsed_time;

void insert_adj_list(int i, int j);
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

    sscanf(s1, "%c %s %d %d", &c, s2, &num_vertex, &num_edge);

    // allocate space to variables;
    dynamic_alloc();

    memset(adj_list, 0, sizeof(VerNode)*(num_vertex + 1));

    while (!feof(fp))
    {
        fscanf(fp, "%c %d %d\n", &c, &i, &j);
        insert_adj_list(i, j);
        insert_adj_list(j, i);
    }

    population.min_conflict = INT_MAX;
    num_color = atoi(argv[2]);

    // this is also the process of initialization;
    for (p = 0; p < num_population; p++)
    {
        memset(adj_color_table, 0, sizeof(adj_color_table));
        memset(tabu_tenure_table, 0, sizeof(tabu_tenure_table));
        f = best_conflict = conflict_num = 0;

        // initialization: set random solution to each solution in the population;
        for (i = 1; i <= num_vertex; i++)
        {
            solution_collection[p][i] = rand() % num_color + 1;
            //cout << solution[i] <<' ';
        }

        // do tabu-search for each population in the collection;
        tabu_search(solution_collection[p]);
        population.num_conflict[p] = f;

        // record the min conflict up till now;
        if (f < population.min_conflict)
        {
            population.min_conflict = f;
            population.min_conflict_index = p;
        }

        if (f == 0)
            break;
    }

    memset(population_solution, 0, sizeof(population_solution));
    double start_time = clock();

    for (p = 0; p < num_population; p++)
    {
        for (i = 1; i <= num_vertex; i++)
        {
            int color = population_solution[p].index1[i] = solution_collection[p][i]; // color of population p, vertex i;
            int color_num = population_solution[p].color_num[color];
            population_solution[p].psol[color][color_num] = i;
            population_solution[p].index2[i] = color_num++;
            population_solution[p].color_num[color] = color_num;
        }
    }

    Population_solution temps;

    while (population.min_conflict != 0)
    {
        // random select two index from population as parents;
        int p1 = rand() % num_population, p2;

        do
        {
            p2 = rand() % num_population;
        } while (p1 == p2);

        memset(&temps, 0, sizeof(temps));

        cross_over(p1, p2, temps.index1);

        // reset adj_color_table and tabu_tenure_table to zero;
        memset(adj_color_table, 0, sizeof(adj_color_table));
        memset(tabu_tenure_table, 0, sizeof(tabu_tenure_table));
        f = best_conflict = conflict_num =0;

        tabu_search(temps.index1); // 仅仅需要对新形成的temps进行禁忌搜索;

        for (i = 1; i <= num_vertex; i++)
        {//变成划分的形式
            int color = temps.index1[i];
            int color_num = temps.color_num[color];
            temps.psol[color][color_num] = i;
            temps.index2[i] = color_num;
            temps.color_num[color] = ++color_num;
        }

        int max_conflict = -1, max_conflict_index;

        for (i = 0; i < num_population; i++)
        {
            if (population.num_conflict[i] > max_conflict)
            {
                max_conflict = population.num_conflict[i];
                max_conflict_index = i;
            }
        }

        population_solution[max_conflict_index] = temps; // 将种群中冲突数最大的替换成temps
        population.num_conflict[max_conflict_index] = f;

        if (f < population.min_conflict)
        {
            population.min_conflict = f;
        }

        cout << "min conflict = " << population.min_conflict << endl;
    }

    double end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    cout << "elapsed time(s): " << elapsed_time << endl;

    if (population.min_conflict == 0)
    {
        fp = fopen("result.txt", "a+");
        if (fp == nullptr)
            printf("output file open error\n");
        fprintf(fp, "%s %-9s %-15lf %-7d\n", argv[1], argv[2], elapsed_time , max_iter);
    }
    else
        cout << "over time" << endl;

    return 0;
}

void insert_adj_list(int i, int j)
{
    ArcNode *temp1 = (ArcNode *)malloc(sizeof(ArcNode));
    temp1->adj_vertex = i;
    temp1->next = adj_list[j].first;
    adj_list[j].first = temp1;
}

void dynamic_alloc()
{
    adj_list = new VerNode [num_vertex + 1];

    solution_collection = new int *[num_population];
    for (int i = 0; i < num_population; i++)
        solution_collection[i] = new int [num_vertex + 1];
}

int tabu_search(int *solution)
{
    bool is_conflict;
    //cout <<endl;

    for (int i = 1; i <= num_vertex; i++)
    {
        ArcNode *temp = adj_list[i].first;
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
            conflict[conflict_num] = i;
            conflict_index[i] = conflict_num++;
        }
    }

    f = f / 2;
    //cout <<"initial_f = "<<f<<endl;
    best_conflict = f;
    iter = 0;

    while (iter < max_iter)
    {
        if (f == 0)
            break;
        Move my_move = find_move(solution);
        make_move(my_move.u, my_move.vj, solution);
        iter++;
    }

    if (iter == max_iter)
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

    for (conf_i = 0; conf_i< conflict_num; conf_i++)
    {
        i = conflict[conf_i];
        sol_i = s[i];
        if (adj_color_table[i][sol_i]>0)
        {
            for (j = 1; j <= num_color; j++)
            {
                if (j != sol_i)
                {
                    int temp_delt = adj_color_table[i][j] - adj_color_table[i][sol_i];
                    //cout <<temp_delt<<'\t';
                    if (iter >= tabu_tenure_table[i][j])
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

void make_move(int u, int vj, int *solultion)
{
    int vi = solultion[u];
    solultion[u] = vj;
    tabu_tenure_table[u][vi] = f + iter + rand() % 10 + 1;
    ArcNode *temp = adj_list[u].first;

    while (temp)
    {
        int adj_vertex = temp->adj_vertex;
        if ((--adj_color_table[adj_vertex][vi]) == 0)
        {
            if (solultion[adj_vertex] == vi)
            {
                delete_conflict(adj_vertex);
            }
        }

        if ((++adj_color_table[adj_vertex][vj]) == 1)
        {
            if (solultion[adj_vertex] == vj)
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
    conflict[conflict_num] = adj_vertex;
    conflict_index[adj_vertex] = conflict_num++;
}

void delete_conflict(int adj_vertex)
{
    int temp_index = conflict_index[adj_vertex];
    conflict[temp_index] = conflict[--conflict_num];
    conflict_index[conflict[temp_index]] = temp_index;
}


void cross_over(int p1, int p2, int *index1)
{
    int A, B;
    Population_solution s[2];
    s[0] = population_solution[p1];
    s[1] = population_solution[p2];

    for (int i = 1; i <= num_color; i++)
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

        int max_index, max_num = -1, *h_num = s[A].color_num;
        for (int j = 1; j <= num_color; j++)
        {
            if (h_num[j] > max_num)
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
            index1[point] = i; //只需要保存哪个点分配了哪种颜色，因为马上要对它进行禁忌搜索，其它的保存了又会变

            int color = s[B].index1[point];//在B中删除这个点
            int index2 = s[B].index2[point];
            int t = s[B].psol[color][index2] = s[B].psol[color][--s[B].color_num[color]];
            s[B].index2[t] = index2;
        }

        //删除这些点
        s[A].color_num[max_index] = 0;

    }

    for (int i = 1; i <= num_color; i++)
    {
        int num = s[0].color_num[i];
        for (int j = 0; j<num; j++)
        {
            int point = s[0].psol[i][j];
            int color = rand() % num_color + 1;//随机分配到某一种颜色中去
            index1[point] = color;
        }
    }
}

