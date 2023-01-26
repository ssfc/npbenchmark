//
// Created by take_ on 2023/1/23.
// hea_vs.cpp : 定义控制台应用程序的入口点。
//


#include "hea.h"

using namespace std;


// random number generator.
mt19937 pseudoRandNumGen;
void init_rand(int seed) { pseudoRandNumGen = mt19937(seed); }


Population_solution::Population_solution()
{}


Population_solution::~Population_solution()
{}


Population::Population(int input_num_population)
{
    min_conflict = INT_MAX;
    num_conflict.resize(input_num_population);
}


Population::~Population()
{

}


Hybrid_Evolution::Hybrid_Evolution(int input_num_vertex, int input_num_color, int input_num_population, int input_seed)
{
    num_vertex = input_num_vertex;
    num_color = input_num_color;
    num_population = input_num_population;

    init_rand(input_seed);

    iter = 0;
    conflict_num = 0;
    max_iter = 16000;

    adj_list.resize(num_vertex + 1);

    adj_color_table.resize(MaxPoint);
    for(int i=0;i<MaxPoint;i++)
    {
        adj_color_table[i].resize(MaxPoint, 0);
    }

    tabu_tenure_table.resize(MaxPoint);
    for(int i=0;i<MaxPoint;i++)
    {
        tabu_tenure_table[i].resize(MaxPoint, 0);
    }

    best_conflict = 0;
    conflict = 0;
    conflict_num = 0;

    conflicts.resize(MaxPoint, 0);
    conflict_index.resize(MaxPoint, 0);

    population_solution.resize(num_population);

    try {
        solution_collection = new int *[num_population];
        for (int i = 0; i < num_population; i++)
            solution_collection[i] = new int [num_vertex + 1];

        // population_solution = new Population_solution [num_population];
    }
    catch (const bad_alloc& e)
    {
        cerr << "初始化内存分配失败:" << endl;
    }
}


Hybrid_Evolution::~Hybrid_Evolution()
{
    for (int i = 0; i < num_population; i++)
        delete[] solution_collection[i];

    // delete []population_solution;
}


void Hybrid_Evolution::insert_adj_list(int i, int j)
{
    ArcNode *temp1 = (ArcNode *)malloc(sizeof(ArcNode));
    temp1->adj_vertex = i;
    temp1->next = adj_list[j].first;
    adj_list[j].first = temp1;
}


void Hybrid_Evolution::tabu_search(int *solution, bool is_limit)
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
                conflict++;
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

    conflict = conflict / 2;
    cerr << "initial_f = " << conflict <<endl;
    best_conflict = conflict;
    iter = 0;

    if(is_limit)
    {
        while (iter < max_iter && conflict > 0)
        {
            Move my_move = find_move(solution);
            make_move(my_move.u, my_move.vj, solution);
            iter++;
        }
    }
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

    int tabu_move_conflict = conflict + tabu_move_delta;
    int non_tabu_move_conflict = conflict + non_tabu_move_delta;

    Move result;

    if (tabu_move_conflict < best_conflict && tabu_move_conflict < non_tabu_move_conflict)
    {
        conflict = tabu_move_conflict;
        best_conflict = tabu_move_conflict;

        int index = pseudoRandNumGen() % tabu_count;
        result = tabu_move[index];
    }
    else
    {
        if (non_tabu_move_conflict < best_conflict)
            best_conflict = non_tabu_move_conflict;
        conflict = non_tabu_move_conflict;
        int index = pseudoRandNumGen() % non_tabu_count;
        result = non_tabu_move[index];
    }

    return result;
}


void Hybrid_Evolution::make_move(int u, int vj, int *solution)
{
    int vi = solution[u];
    solution[u] = vj;
    tabu_tenure_table[u][vi] = conflict + iter + pseudoRandNumGen() % 10 + 1;
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

        int max_index;
        int max_num = -1;
        int *h_num = s[A].color_num;

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

            int color = s[B].index1s[point];//在B中删除这个点
            int index2 = s[B].index2s[point];

            s[B].psol[color][index2] = s[B].psol[color][--s[B].color_num[color]];
            int t = s[B].psol[color][index2];
            s[B].index2s[t] = index2;
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
            int color = pseudoRandNumGen() % num_color;//随机分配到某一种颜色中去
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

            temp = temp->next;
        }
    }

    this_conflict = this_conflict / 2;

    return this_conflict;
}


// debug function
long long int Hybrid_Evolution::get_iteration() const
{
    return iter;
}


/*
 * argv[1]: to be added;
 * argv[2]: random seed;
 * argv[3]: file name;
*/
int main(int argc, char *argv[])
{
    int num_population = 20;
    Population population(num_population);

    int i, j, p;
    char file[100];
    FILE *fp;

    sprintf(file, "./data/%s", argv[3]);

    if ((fp = fopen(file, "r")) == nullptr)
    {
        cerr << "file not open";
        return 0;
    }

    int input_num_vertex;
    int num_edge;
    int input_num_color;

    fscanf(fp, "%d %d %d\n", &input_num_vertex, &num_edge, &input_num_color);

    int rand_seed = atoi(argv[2]);

    Hybrid_Evolution test(input_num_vertex, input_num_color, num_population, rand_seed);

    memset(&test.adj_list[0], 0, sizeof(test.adj_list[0]) * test.adj_list.size());

    while (!feof(fp))
    {
        fscanf(fp, "%d %d\n", &i, &j);
        test.insert_adj_list(i+1, j+1);
        test.insert_adj_list(j+1, i+1);
    }

    /*// to debug
    int *temp_solution = new int [test.num_vertex + 1];
    for(auto& x : test.adj_color_table) memset(&x[0],0,sizeof(int)*x.size());
    for(auto& x : test.tabu_tenure_table) memset(&x[0],0,sizeof(int)*x.size());


    // initialization: set random solution to each solution in the population;
    for (i = 1; i <= test.num_vertex; i++)
    {
        temp_solution[i] = pseudoRandNumGen() % test.num_color;
        //cerr << solution[i] <<' ';
    }

    // do tabu-search for each population in the collection;
    cerr << "Conflict before tabu search is: " << test.compute_conflict(temp_solution) << endl;
    // cerr << "iterations: " << test.get_iteration() << endl;

    double start_time = clock();

    test.tabu_search(temp_solution);

    double end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    // cerr << "elapsed time(s): " << elapsed_time << endl;

    cerr << "Iterations: " << test.get_iteration() << " elapsed_time(s): " << elapsed_time
         << " frequency:" << double (test.get_iteration()) / elapsed_time << endl;

    cerr << "Conflict after tabu search is: " << test.compute_conflict(temp_solution) << endl;
    // cerr << "iterations: " << test.get_iteration() << endl;
     */


    ///* to reduce
    // this is also the process of initialization;
    for (p = 0; p < num_population; p++)
    {
        for(auto& x : test.adj_color_table) memset(&x[0],0,sizeof(int)*x.size());
        for(auto& x : test.tabu_tenure_table) memset(&x[0],0,sizeof(int)*x.size());

        test.conflict = 0;
        test.best_conflict = 0;
        test.conflict_num = 0;

        // initialization: set random solution to each solution in the population;
        for (i = 1; i <= test.num_vertex; i++)
        {
            test.solution_collection[p][i] = pseudoRandNumGen() % test.num_color;
            //cerr << solution[i] <<' ';
        }

        // do tabu-search for each population in the collection;
        // cerr << "Conflict before tabu search is: " << test.compute_conflict(test.solution_collection[p]) << endl;
        test.tabu_search(test.solution_collection[p], true);
        // cerr << "Conflict after tabu search is: " << test.compute_conflict(test.solution_collection[p]) << endl;

        population.num_conflict[p] = test.conflict;

        // record the min conflict up till now;
        if (test.conflict < population.min_conflict)
        {
            population.min_conflict = test.conflict;
            population.min_conflict_index = p;
        }

        if (test.conflict == 0)
            break;

    }

    memset(&test.population_solution[0], 0, sizeof(test.population_solution[0])*test.population_solution.size());
    double start_time = clock();

    for (p = 0; p < num_population; p++)
    {
        for (i = 1; i <= test.num_vertex; i++)
        {
            test.population_solution[p].index1s[i] = test.solution_collection[p][i]; // color of population p, vertex i;
            int color = test.solution_collection[p][i]; // color of population p, vertex i;
            int color_num = test.population_solution[p].color_num[color];

            test.population_solution[p].psol[color][color_num] = i;
            test.population_solution[p].index2s[i] = color_num++;
            test.population_solution[p].color_num[color] = color_num;
        }
    }


    Population_solution temps;

    long long int population_iteration = 0;
    while (population.min_conflict != 0)
    {
        // random select two index from population as parents;
        int p1 = pseudoRandNumGen() % num_population, p2;

        do
        {
            p2 = pseudoRandNumGen() % num_population;
        } while (p1 == p2);

        memset(&temps, 0, sizeof(temps));

        test.cross_over(p1, p2, temps.index1s);

        // reset adj_color_table and tabu_tenure_table to zero;
        for(auto& x : test.adj_color_table) memset(&x[0],0,sizeof(int)*x.size());
        for(auto& x : test.tabu_tenure_table) memset(&x[0],0,sizeof(int)*x.size());

        test.conflict = 0;
        test.best_conflict = 0;
        test.conflict_num = 0;

        test.tabu_search(temps.index1s, true); // 仅仅需要对新形成的temps进行禁忌搜索;

        for (i = 1; i <= test.num_vertex; i++)
        {//变成划分的形式
            int color = temps.index1s[i];
            int color_num = temps.color_num[color];
            temps.psol[color][color_num] = i;
            temps.index2s[i] = color_num;
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
        population.num_conflict[max_conflict_index] = test.conflict;

        if (test.conflict < population.min_conflict)
        {
            population.min_conflict = test.conflict;
            population.min_conflict_index = max_conflict_index;
        }

        cerr << "min conflict = " << population.min_conflict << endl;
        cerr << "min conflict index = " << population.min_conflict_index << endl;
        population_iteration++;
    }


    double end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    cerr << "Population iteration: " << population_iteration << " ";
    cerr << "elapsed time(s): " << elapsed_time << " ";
    cerr << "Population frequency: " << double (population_iteration) / elapsed_time << endl;

    if (population.min_conflict == 0)
    {
        fp = fopen("result.txt", "a+");
        if (fp == nullptr)
            printf("output file open error\n");
        fprintf(fp, "%s %-9d %-15lf %-7lld\n", argv[3], test.num_color, elapsed_time , test.max_iter);

        cerr << "color of each vertex: ";
        for(i=1;i<=test.num_vertex;i++)
        {
            cerr << test.population_solution[population.min_conflict_index].index1s[i] << " ";
        }
        cerr << endl;

        cerr << "conflict of solution 19: ";
        cerr << test.compute_conflict(test.population_solution[19].index1s) << endl;

        cerr << "conflict of final solution: ";
        cerr << test.compute_conflict(test.population_solution[population.min_conflict_index].index1s) << endl;
    }
    else
        cerr << "over time" << endl;
        //*/ to reduce


    return 0;
}

// debugging command:
// g++ hea.cpp -g; gdb a.out
// r 11 6 chvatal.txt