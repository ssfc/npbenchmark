//
// Created by take_ on 2022/12/30.
//
// hea_vs.cpp : 定义控制台应用程序的入口点。
//

#include "hea.h"

using namespace std;


int tabu_tenure_table[MaxPoint][MaxPoint];

Population_solution::Population_solution()
{}

Population_solution::~Population_solution()
{}

Population::Population(int input_num_population)
{
    min_conflict = INT_MAX;


    num_conflict = new int [input_num_population];
}

Population::~Population()
{
    delete []num_conflict;
}

Hybrid_Evolution::Hybrid_Evolution(int input_num_vertex, int input_num_color, int input_num_population)
{
    num_vertex = input_num_vertex;
    num_color = input_num_color;
    num_population = input_num_population;

    iter = 0;
    conflict_num = 0;
    max_iter = 16000;

    try {
        adj_list = new VerNode[num_vertex + 1];

        /*
        adj_color_table = new int* [MaxPoint];

        for (int i = 0; i < MaxPoint; i++)
        {
            adj_color_table[i] = new int [MaxPoint];
        }
        */


        solution_collection = new int *[num_population];
        for (int i = 0; i < num_population; i++)
            solution_collection[i] = new int[num_vertex + 1];

        population_solution = new Population_solution[num_population];

        conflicts = new int[MaxPoint];
        conflict_index = new int[MaxPoint];
    }
    catch (const bad_alloc& e)
    {
        cerr << "初始化内存分配失败:" << endl;
    }
}


Hybrid_Evolution::~Hybrid_Evolution()
{
    delete []adj_list;

    for (int i = 0; i < num_population; i++)
        delete[] solution_collection[i];

    delete []population_solution;

    delete []conflicts;
    delete []conflict_index;
}

void Hybrid_Evolution::insert_adj_list(int i, int j) const
{
    ArcNode *temp1 = (ArcNode *)malloc(sizeof(ArcNode));
    temp1->adj_vertex = i;
    temp1->next = adj_list[j].first;
    adj_list[j].first = temp1;
}


int Hybrid_Evolution::tabu_search(int *solution)
{
    bool is_conflict;
    //cerr <<endl;

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
            conflicts[conflict_num] = i;
            conflict_index[i] = conflict_num++;
        }
    }

    f = f / 2;
    cerr <<"initial_f = "<<f<<endl;
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

    return 1;
}


Move Hybrid_Evolution::find_move(const int *solution)
{
    int sol_i;

    Move tabu_move[MaxPoint];
    Move non_tabu_move[MaxPoint];

    int tabu_count = 1;
    int non_tabu_count = 1;

    int tabu_move_delta = 100000;
    int non_tabu_move_delta = 100000;

    for (int conf_i = 0; conf_i< conflict_num; conf_i++)
    {
        int i = conflicts[conf_i];
        sol_i = solution[i];
        if (adj_color_table[i][sol_i]>0)
        {
            for (int j = 0; j < num_color; j++)
            {
                if (j != sol_i)
                {
                    int this_delta = adj_color_table[i][j] - adj_color_table[i][sol_i];
                    //cerr <<this_delta<<'\t';
                    if (iter >= tabu_tenure_table[i][j])
                    {
                        if (this_delta <= non_tabu_move_delta)
                        {
                            if (this_delta < non_tabu_move_delta)
                            {
                                non_tabu_count = 0;
                                non_tabu_move_delta = this_delta;
                            }

                            non_tabu_move[non_tabu_count].u = i;
                            non_tabu_move[non_tabu_count++].vj = j;
                        }
                    }
                    else
                    {
                        if (this_delta <= tabu_move_delta)
                        {
                            if (this_delta < tabu_move_delta)
                            {
                                tabu_count = 0;
                                tabu_move_delta = this_delta;
                            }

                            tabu_move[tabu_count].u = i;
                            tabu_move[tabu_count++].vj = j;
                        }
                    }
                }
            }
        }
    }

    int temp1 = f + tabu_move_delta, temp2 = f + non_tabu_move_delta;
    Move res;
    if (temp1<best_conflict && temp1<temp2)
    {
        f = best_conflict = temp1;
        int index = rand() % tabu_count;
        res = tabu_move[index];
    }
    else
    {
        if (temp2 < best_conflict)
            best_conflict = temp2;
        f = temp2;
        int index = rand() % non_tabu_count;
        res = non_tabu_move[index];
    }

    return res;
}

void Hybrid_Evolution::make_move(int u, int vj, int *solution)
{
    int vi = solution[u];
    solution[u] = vj;
    tabu_tenure_table[u][vi] = f + iter + rand() % 10 + 1;
    ArcNode *temp = adj_list[u].first;

    while (temp)
    {
        int adj_vertex = temp->adj_vertex;
        if ((--adj_color_table[adj_vertex][vi]) == 0)
        {
            if (solution[adj_vertex] == vi)
            {
                delete_conflict(adj_vertex);
            }
        }

        if ((++adj_color_table[adj_vertex][vj]) == 1)
        {
            if (solution[adj_vertex] == vj)
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

void Hybrid_Evolution::add_conflict(int adj_vertex)
{
    conflicts[conflict_num] = adj_vertex;
    conflict_index[adj_vertex] = conflict_num++;
}

void Hybrid_Evolution::delete_conflict(int adj_vertex)
{
    int temp_index = conflict_index[adj_vertex];
    conflicts[temp_index] = conflicts[--conflict_num];
    conflict_index[conflicts[temp_index]] = temp_index;
}

void Hybrid_Evolution::cross_over(int p1, int p2, int *index1) const
{
    int A, B;
    Population_solution s[2];
    s[0] = population_solution[p1];
    s[1] = population_solution[p2];

    for (int i = 0; i < num_color; i++)
    {
        if (i % 2 != 1)
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
        for (int j = 0; j < num_color; j++)
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

    for (int i = 0; i < num_color; i++)
    {
        int num = s[0].color_num[i];
        for (int j = 0; j<num; j++)
        {
            int point = s[0].psol[i][j];
            int color = rand() % num_color;//随机分配到某一种颜色中去
            index1[point] = color;
        }
    }
}

// debug function: compute conflict of a solution
int Hybrid_Evolution::compute_conflict(const int *solution)
{
    int this_conflict = 0;
    //cerr <<endl;

    for (int i = 1; i <= num_vertex; i++)
    {
        ArcNode *temp = adj_list[i].first;
        while (temp)
        {
            if (solution[temp->adj_vertex] == solution[i])
            {
                this_conflict++;
            }
            adj_color_table[i][solution[temp->adj_vertex]]++;
            temp = temp->next;
        }
    }

    this_conflict = this_conflict / 2;

    return this_conflict;
}


/*
argv[1]:文件名
*/
int main(int argc, char *argv[])
{
    int num_population = 20;
    Population population(num_population);

    int i, j, p;
    char file[100];
    FILE *fp;
    srand(6);

    sprintf(file, "./data/%s", argv[1]);

    if ((fp = fopen(file, "r")) == nullptr)
    {
        cerr << "file not open";
        return 0;
    }

    int input_num_vertex;
    int num_edge;
    int input_num_color;

    fscanf(fp, "%d %d %d\n", &input_num_vertex, &num_edge, &input_num_color);
    Hybrid_Evolution test(input_num_vertex, input_num_color, num_population);

    memset(test.adj_list, 0, sizeof(VerNode)*(test.num_vertex + 1));

    while (!feof(fp))
    {
        fscanf(fp, "%d %d\n", &i, &j);
        test.insert_adj_list(i+1, j+1);
        test.insert_adj_list(j+1, i+1);
    }
    

    // this is also the process of initialization;
    for (p = 0; p < num_population; p++)
    {
        memset(test.adj_color_table, 0, sizeof(int)*MaxPoint*MaxPoint);
        memset(tabu_tenure_table, 0, sizeof(int)*MaxPoint*MaxPoint);
        test.f = test.best_conflict = test.conflict_num = 0;

        // initialization: set random solution to each solution in the population;
        for (i = 1; i <= test.num_vertex; i++)
        {
            test.solution_collection[p][i] = rand() % test.num_color;
            //cerr << solution[i] <<' ';
        }

        // do tabu-search for each population in the collection;
        // cerr << "Compute conflict is: " << compute_conflict(solution_collection[p]) << endl;
        test.tabu_search(test.solution_collection[p]);

        population.num_conflict[p] = test.f;

        // record the min conflict up till now;
        if (test.f < population.min_conflict)
        {
            population.min_conflict = test.f;
            population.min_conflict_index = p;
        }

        if (test.f == 0)
            break;
    }

    memset(test.population_solution, 0, sizeof(test.population_solution));
    double start_time = clock();

    for (p = 0; p < num_population; p++)
    {
        for (i = 1; i <= test.num_vertex; i++)
        {
            int color = test.population_solution[p].index1[i] = test.solution_collection[p][i]; // color of population p, vertex i;
            int color_num = test.population_solution[p].color_num[color];
            test.population_solution[p].psol[color][color_num] = i;
            test.population_solution[p].index2[i] = color_num++;
            test.population_solution[p].color_num[color] = color_num;
        }
    }

    Population_solution temps;

    int population_iteration = 0;
    while (population.min_conflict != 0)
    {
        // random select two index from population as parents;
        int p1 = rand() % num_population, p2;

        do
        {
            p2 = rand() % num_population;
        } while (p1 == p2);

        memset(&temps, 0, sizeof(temps));

        test.cross_over(p1, p2, temps.index1);

        // reset adj_color_table and tabu_tenure_table to zero;
        memset(test.adj_color_table, 0, sizeof(int)*MaxPoint*MaxPoint);
        memset(tabu_tenure_table, 0, sizeof(int)*MaxPoint*MaxPoint);
        test.f = test.best_conflict = test.conflict_num =0;

        test.tabu_search(temps.index1); // 仅仅需要对新形成的temps进行禁忌搜索;

        for (i = 1; i <= test.num_vertex; i++)
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

        test.population_solution[max_conflict_index] = temps; // 将种群中冲突数最大的替换成temps
        population.num_conflict[max_conflict_index] = test.f;

        if (test.f < population.min_conflict)
        {
            population.min_conflict = test.f;
            population.min_conflict_index = max_conflict_index;
        }

        cerr << "min conflict = " << population.min_conflict << endl;
        cerr << "min conflict index = " << population.min_conflict_index << endl;
        population_iteration++;
    }

    double end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    cerr << "Population iteration: " << population_iteration << endl;
    cerr << "elapsed time(s): " << elapsed_time << endl;

    if (population.min_conflict == 0)
    {
        fp = fopen("result.txt", "a+");
        if (fp == nullptr)
            printf("output file open error\n");
        fprintf(fp, "%s %-9d %-15lf %-7d\n", argv[1], test.num_color, elapsed_time , test.max_iter);

        cerr << "color of each vertex: ";
        for(i=1;i<=test.num_vertex;i++)
        {
            cerr << test.population_solution[population.min_conflict_index].index1[i] << " ";
        }
        cerr << endl;

        cerr << "conflict of solution 19: ";
        cerr << test.compute_conflict(test.population_solution[19].index1) << endl;

        cerr << "conflict of final solution: ";
        cerr << test.compute_conflict(test.population_solution[population.min_conflict_index].index1) << endl;
    }
    else
        cerr << "over time" << endl;

    return 0;
}