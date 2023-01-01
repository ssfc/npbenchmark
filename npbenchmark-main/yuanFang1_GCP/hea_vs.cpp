//
// Created by take_ on 2022/12/30.
//
// hea_vs.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>

#define MaxPoint 1002
#define MaxColor 300
#define P 20
using namespace std;
int MaxIter = 16000;

struct P_sol {
    int psol[MaxColor][MaxPoint];
    int num[MaxColor];
    int index1[MaxPoint], index2[MaxPoint];
};

struct Population {
    int min_f, min_p;
    int fnum[P + 1];
};

Population popu;
P_sol p_sol[P+1];
int conflict[MaxPoint], conf_index[MaxPoint];
int conf_num = 0;
typedef struct ArcNode {
    int adjvex;
    struct ArcNode *next;
}ArcNode;

typedef struct VerNode {
    ArcNode *first;
}VerNode;

typedef struct Move {
    int u, vi, vj;
}Move;
VerNode *adjList;
int **sol;
int adj_color_table[MaxPoint][MaxPoint], tabu_table[MaxPoint][MaxPoint];
int point_num, edge_num, f, best_f, k;
long long iter;
int res_iter;
double res_time;

void insert_adjList(int i, int j);
void dynamic_alloc();
int tabu_search(int *s);
Move FindMove(int *s);
void MakeMove(int u, int vj, int *s);
void add_conf(int adjvex);
void del_conf(int adjvex);
void crossover(int p1, int p2, int *index1);
/*
argv[1]:文件名
argv[2]:迭代次数
argv[3]:颜色数
*/
int main(int argc, char *argv[])
{
    int i, j, p;
    char c, s1[100], s2[100], file[100];
    FILE *fp;
    srand((unsigned)time(NULL));
    sprintf(file, "./instances/%s", argv[1]);
    if ((fp = fopen(file, "r")) == NULL) {
        cout << "file not open";
        return 0;
    }
    do {
        fgets(s1, 100, fp);
    } while (s1[0] == 'c');
    sscanf(s1, "%c %s %d %d", &c, s2, &point_num, &edge_num);

    dynamic_alloc();
    memset(adjList, 0, sizeof(VerNode)*(point_num + 1));
    while (!feof(fp)) {
        fscanf(fp, "%c %d %d\n", &c, &i, &j);
        insert_adjList(i, j);
        insert_adjList(j, i);
    }

    popu.min_f = 100000;
    k = atoi(argv[2]);

    for (p = 1; p <= P; p++) {
        memset(adj_color_table, 0, sizeof(adj_color_table));
        memset(tabu_table, 0, sizeof(tabu_table));
        f = best_f=conf_num = 0;
        for (i = 1; i <= point_num; i++) {
            sol[p][i] = rand() % k + 1;
            //cout << sol[i] <<' ';
        }
        tabu_search(sol[p]);
        popu.fnum[p] = f;

        if (f < popu.min_f) {
            popu.min_f = f;
            popu.min_p = p;
        }
        if (f == 0)
            break;
    }
    memset(p_sol, 0, sizeof(p_sol));
    clock_t start = clock();
    if (p > P) {
        for (p = 1; p <= P; p++) {
            for (i = 1; i <= point_num; i++) {
                int color = p_sol[p].index1[i] = sol[p][i];
                int color_num = p_sol[p].num[color];
                p_sol[p].psol[color][color_num] = i;
                p_sol[p].index2[i] = color_num++;
                p_sol[p].num[color] = color_num;
            }
        }
        P_sol temps;
        int cnt = 0;
        while (popu.min_f != 0) {
            int p1 = rand() % P + 1, p2;
            do {
                p2 = rand() % P + 1;
            } while (p1 == p2);
            memset(&temps, 0, sizeof(temps));

            crossover(p1, p2, temps.index1);

            memset(adj_color_table, 0, sizeof(adj_color_table));
            memset(tabu_table, 0, sizeof(tabu_table));
            f = best_f = conf_num =0;

            tabu_search(temps.index1);//对temps进行禁忌搜索



            for (i = 1; i <= point_num; i++) {//变成划分的形式
                int color = temps.index1[i];
                int color_num = temps.num[color];
                temps.psol[color][color_num] = i;
                temps.index2[i] = color_num;
                temps.num[color] = ++color_num;
            }
            int max_f = -1, max_p;
            for (i = 1; i <= P; i++) {
                if (popu.fnum[i] >max_f) {
                    max_f = popu.fnum[i];
                    max_p = i;
                }
            }

            p_sol[max_p] = temps;//将种群中冲突数最大的替换成temps
            popu.fnum[max_p] = f;

            if (f<popu.min_f) {
                popu.min_f = f;
                popu.min_p = max_p;
            }

            cout << "mint_f = " << popu.min_f << endl;
        }
    }
    clock_t ends = clock();
    res_time = (double)(ends - start) / CLOCKS_PER_SEC;
    cout << res_time<<endl;
    if (popu.min_f == 0) {
        fp = fopen(".\\result.txt", "a+");
        if (fp == NULL)
            printf("output file open error\n");
        fprintf(fp, "%s %-9s %-15lf %-7d\n", argv[1], argv[2], res_time , MaxIter);
    }
    else
        cout << "overtime" << endl;

    return 0;
}

void insert_adjList(int i, int j) {
    ArcNode *temp1 = (ArcNode *)malloc(sizeof(ArcNode));
    temp1->adjvex = i;
    temp1->next = adjList[j].first;
    adjList[j].first = temp1;
}

void dynamic_alloc()
{
    int i;
    adjList = (VerNode *)malloc(sizeof(VerNode)*(point_num + 1));
    //p_sol = (P_sol *)malloc(sizeof(P_sol)*(P + 1));
    sol = (int **)malloc(sizeof(int *)*(P + 1));
    for (i = 0; i <= P; i++)
        sol[i] = (int *)malloc(sizeof(int)*(point_num + 1));
}

int tabu_search(int *s)
{
    int i, is_conf;
    //cout <<endl;

    for (i = 1; i <= point_num; i++) {
        ArcNode *temp = adjList[i].first;
        is_conf = 0;
        while (temp) {
            if (s[temp->adjvex] == s[i]) {
                is_conf = 1;
                f++;
            }
            adj_color_table[i][s[temp->adjvex]]++;
            temp = temp->next;
        }
        if (is_conf) {
            conflict[conf_num] = i;
            conf_index[i] = conf_num++;
        }
    }
    f = f / 2;
    //cout <<"initial_f = "<<f<<endl;
    best_f = f;
    iter = 0;

    while (iter < MaxIter) {
        if (f == 0)
            break;
        Move mymove = FindMove(s);
        MakeMove(mymove.u, mymove.vj, s);
        iter++;
    }

    if (iter == MaxIter)
        return 0;

    res_iter = iter;

    return 1;
}

Move FindMove(int *s) {
    int i, j, conf_i;
    int sol_i;
    Move tabu_move[MaxPoint], non_tabu_move[MaxPoint];
    int tabu_cnt = 1, non_tabu_cnt = 1;
    int tabu_move_delt = 100000, non_tabu_move_delt = 100000;
    for (conf_i = 0; conf_i< conf_num; conf_i++) {
        i = conflict[conf_i];
        sol_i = s[i];
        if (adj_color_table[i][sol_i]>0) {
            for (j = 1; j <= k; j++) {
                if (j != sol_i) {
                    int temp_delt = adj_color_table[i][j] - adj_color_table[i][sol_i];
                    //cout <<temp_delt<<'\t';
                    if (iter >= tabu_table[i][j]) {
                        if (temp_delt <= non_tabu_move_delt) {
                            if (temp_delt < non_tabu_move_delt) {
                                non_tabu_cnt = 0;
                                non_tabu_move_delt = temp_delt;
                            }
                            non_tabu_move[non_tabu_cnt].u = i;
                            non_tabu_move[non_tabu_cnt++].vj = j;
                        }
                        //	                    else if(temp_delt == non_tabu_move.delt){
                        //	                    	non_tabu_cnt++;
                        //	                    	double t = 1.0/non_tabu_cnt;
                        //	                    	if((rand()/(RAND_MAX+1.0)) < t){
                        //	                    		non_tabu_move.u = i;
                        //		                        non_tabu_move.vi = sol_i;
                        //		                        non_tabu_move.vj = j;
                        //							}
                        //
                        //						}
                    }
                    else {
                        if (temp_delt <= tabu_move_delt) {
                            if (temp_delt < tabu_move_delt) {
                                tabu_cnt = 0;
                                tabu_move_delt = temp_delt;
                            }
                            tabu_move[tabu_cnt].u = i;
                            tabu_move[tabu_cnt++].vj = j;
                        }
                        //	                    else if(temp_delt == tabu_move.delt){
                        //	                    	tabu_cnt++;
                        //	                    	double t = 1.0/tabu_cnt,r= rand()/(RAND_MAX+1.0);
                        //	                    	if(r < t){
                        //	                    		tabu_move.u = i;
                        //		                        tabu_move.vi = sol_i;
                        //		                        tabu_move.vj = j;
                        //							}
                        //
                        //						}

                    }
                }

            }
        }
    }
    int temp1 = f + tabu_move_delt, temp2 = f + non_tabu_move_delt;
    Move res;
    if (temp1<best_f && temp1<temp2) {
        f = best_f = temp1;
        int index = rand() % tabu_cnt;
        res = tabu_move[index];
    }
    else {
        if (temp2<best_f)
            best_f = temp2;
        f = temp2;
        int index = rand() % non_tabu_cnt;
        res = non_tabu_move[index];
    }
    return res;

}
void MakeMove(int u, int vj, int *s) {
    int vi = s[u];
    s[u] = vj;
    tabu_table[u][vi] = f + iter + rand() % 10 + 1;
    ArcNode *temp = adjList[u].first;

    while (temp) {
        int adjvex = temp->adjvex;
        if ((--adj_color_table[adjvex][vi]) == 0) {
            if (s[adjvex] == vi) {
                del_conf(adjvex);
            }
        }
        if ((++adj_color_table[adjvex][vj]) == 1) {
            if (s[adjvex] == vj) {
                add_conf(adjvex);
            }
        }
        temp = temp->next;
    }
    if (adj_color_table[u][vi] != 0 && adj_color_table[u][vj] == 0)
        del_conf(u);
    if (adj_color_table[u][vi] == 0 && adj_color_table[u][vj] != 0)
        add_conf(u);
}

void add_conf(int adjvex) {
    conflict[conf_num] = adjvex;
    conf_index[adjvex] = conf_num++;
}

void del_conf(int adjvex) {
    int temp_index = conf_index[adjvex];
    conflict[temp_index] = conflict[--conf_num];
    conf_index[conflict[temp_index]] = temp_index;
}


void crossover(int p1, int p2, int *index1) {
    int l, A, B, j;
    P_sol s[2];
    s[0] = p_sol[p1];
    s[1] = p_sol[p2];
    for (l = 1; l <= k; l++) {
        if (l % 2 != 0) {
            A = 0; B = 1;
        }
        else {
            A = 1; B = 0;
        }
        int max_index, max_num = -1, *h_num = s[A].num;
        for (j = 1; j <= k; j++) {
            if (h_num[j] >max_num) {
                max_index = j;
                max_num = h_num[j];
            }
        }
        int num = h_num[max_index];
        int *h_color = s[A].psol[max_index];
        for (j = 0; j<num; j++) {
            int point = h_color[j];
            index1[point] = l;//只需要保存哪个点分配了哪种颜色，因为马上要对它进行禁忌搜索，其它的保存了又会变

            int color = s[B].index1[point];//在B中删除这个点
            int index2 = s[B].index2[point];
            int t = s[B].psol[color][index2] = s[B].psol[color][--s[B].num[color]];
            s[B].index2[t] = index2;
        }

        //删除这些点
        s[A].num[max_index] = 0;

    }
    for (l = 1; l <= k; l++) {
        int num = s[0].num[l];
        for (j = 0; j<num; j++) {
            int point = s[0].psol[l][j];
            int color = rand() % k + 1;//随机分配到某一种颜色中去
            index1[point] = color;
        }
    }
}