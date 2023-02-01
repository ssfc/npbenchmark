#include "hea.h"

using namespace std;


// random number generator.
mt19937 pseudoRandNumGen;
void init_rand(int seed) { pseudoRandNumGen = mt19937(seed); }


Population_solution::Population_solution(int input_num_color)
{
    psol.resize(input_num_color);
    for(auto & i : psol)
    {
        i.resize(MaxPoint, 0);
    }

    color_num.resize(input_num_color, 0);
    index1s.resize(MaxPoint, 0);
    index2s.resize(MaxPoint, 0);
}


Population_solution::~Population_solution()
= default;


Population::Population(int input_num_population)
{
    min_conflict = INT_MAX;
    min_conflict_index = 0;
    num_conflict.resize(input_num_population);
}


Population::~Population()
= default;


Hybrid_Evolution::Hybrid_Evolution(int input_num_vertex, int input_num_color, int input_num_population, int input_seed)
{
    init_rand(input_seed);

    num_vertex = input_num_vertex;
    num_color = input_num_color;
    adj_yf_list.resize(num_vertex + 1);
    adj_list.resize(num_vertex + 1);
    for (int i = 1; i <= num_vertex; i++)
    {
        adj_list[i].resize(num_vertex + 1, 0);
    }
    vertex_edge_num.resize(num_vertex + 1, 0);

    conflict = 0;
    best_conflict = 0;
    conflict_num = 0;
    conflicts.resize(MaxPoint, 0);
    conflict_index.resize(MaxPoint, 0);

    adj_color_table.resize(MaxPoint);
    for(auto & i : adj_color_table)
    {
        i.resize(input_num_color, 0);
    }

    tabu_tenure_table.resize(MaxPoint);
    for(auto & i : tabu_tenure_table)
    {
        i.resize(input_num_color, 0);
    }

    moved = {-1, -1};
    tabu_move.resize(2000, {-1, -1});
    non_tabu_move.resize(2000, {-1, -1});
    iter = 0;
    max_iter = 16000;

    min_delta = 999999;

    equal_nontabu_delta.resize(2000, {-1, -1});
    equal_tabu_delta.resize(2000, {0,0});

    single_solution.resize(num_vertex + 1);
    for (int i = 1; i <= num_vertex; i++)
        single_solution[i] = pseudoRandNumGen() % num_color;//初始化颜色

    num_population = input_num_population;
    solution_collection.resize(num_population);
    for (auto & i : solution_collection)
        i.resize(num_vertex + 1, 0);
    population_solution.resize(num_population, Population_solution(input_num_color));

    // debug variables;
    max_equal_nontabu_count = 0;
    max_equal_tabu_count = 0;
    start_time = 0;
    end_time = 0;
}


Hybrid_Evolution::~Hybrid_Evolution()
= default;


void Hybrid_Evolution::insert_adj_yf_list(int i, int j)
{
    auto *temp1 = (ArcNode *)malloc(sizeof(ArcNode));
    temp1->adj_vertex = i;
    temp1->next = adj_yf_list[j].first;
    adj_yf_list[j].first = temp1;
}


void Hybrid_Evolution::insert_adj_list(int v1, int v2)
{
    adj_list[v1][vertex_edge_num[v1]] = v2;
    vertex_edge_num[v1]++;

    adj_list[v2][vertex_edge_num[v2]] = v1;
    vertex_edge_num[v2]++;
}


void Hybrid_Evolution::find_yf_move(vector<unsigned int> &solution)
{
    unsigned int sol_i;

    int tabu_count = 0;
    int non_tabu_count = 0;

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

    if (tabu_move_conflict < best_conflict && tabu_move_conflict < non_tabu_move_conflict)
    {
        conflict = tabu_move_conflict;
        best_conflict = tabu_move_conflict;

        unsigned int index = pseudoRandNumGen() % tabu_count;
        moved = tabu_move[index];
    }
    else
    {
        if (non_tabu_move_conflict < best_conflict)
            best_conflict = non_tabu_move_conflict;
        conflict = non_tabu_move_conflict;
        unsigned int index = pseudoRandNumGen() % non_tabu_count;
        moved = non_tabu_move[index];
    }
}


void Hybrid_Evolution::find_move()
{
    min_delta = 999999;
    int tabu_delta = 999999;
    int equal_nontabu_count = 0;
    int equal_tabu_count = 0;

    int aspiration = best_conflict - conflict;

    for (int i = 1; i <= num_vertex; i++) // i is vertex;
    {
        unsigned int solution_i = single_solution[i]; // solution_i is color;

        if (adj_color_table[i][solution_i] > 0) // if vertex i overlap its neighbor's color;
        {
            for (int j = 0; j < num_color; j++) // j is color;
            {
                if (solution_i != j) // find a new color;
                {//cpu流水线
                    int this_delta = adj_color_table[i][j] - adj_color_table[i][solution_i]; // new-old, the less the better;
                    if (tabu_tenure_table[i][j] <= iter) //nontabu move;
                    {
                        if (this_delta < min_delta)
                        {//分支预判惩罚 6.0
                            equal_nontabu_count = 0;
                            min_delta = this_delta;

                            equal_nontabu_delta[equal_nontabu_count].u = i; // i is vertex;
                            equal_nontabu_delta[equal_nontabu_count].vj = j; // j is color;

                            equal_nontabu_count++;
                        }
                        else if (this_delta == min_delta)
                        {
                            equal_nontabu_delta[equal_nontabu_count].u = i; // i is vertex;
                            equal_nontabu_delta[equal_nontabu_count].vj = j; // j is color;

                            equal_nontabu_count++;
                        }

                        // debug part: estimate the size of max equal nontabu count;
                        if(max_equal_nontabu_count < equal_nontabu_count)
                        {
                            max_equal_nontabu_count = equal_nontabu_count;
                            cerr << "max equal nontabu count: " << max_equal_nontabu_count << endl;
                        }
                        // debug part end;
                    }
                    else // tabu move;
                    {
                        if (this_delta < tabu_delta)
                        {//6.0
                            equal_tabu_count = 0;
                            tabu_delta = this_delta;

                            equal_tabu_delta[equal_tabu_count].u = i; // i is vertex;
                            equal_tabu_delta[equal_tabu_count].vj = j; // j is color;

                            equal_tabu_count++;
                        }
                        else if(this_delta == tabu_delta)
                        {
                            equal_tabu_delta[equal_tabu_count].u = i; // i is vertex;
                            equal_tabu_delta[equal_tabu_count].vj = j; // j is color;

                            equal_tabu_count++;
                        }

                        // debug part: estimate the size of max equal tabu count;
                        if(max_equal_tabu_count < equal_tabu_count)
                        {
                            max_equal_tabu_count = equal_tabu_count;
                            cerr << "max equal tabu count: " << max_equal_tabu_count << endl;
                        }
                        // debug part end;
                    }
                }
            }
        }
    }

    if (tabu_delta < aspiration && tabu_delta < min_delta)
    {
        min_delta = tabu_delta;
        unsigned int rand_select = pseudoRandNumGen() % equal_tabu_count; // 相等tabu_delta随机选择
        // cerr << "random select tabu: " << rand_select << endl;
        moved = equal_tabu_delta[rand_select];
    }
    else
    {
        unsigned int rand_select = pseudoRandNumGen() % equal_nontabu_count; // 相等delta随机选择
        // cerr << "random select nontabu: " << rand_select << endl;
        moved = equal_nontabu_delta[rand_select];
    }
}


void Hybrid_Evolution::make_yf_move(vector<unsigned int> &solution)
{
    unsigned int vi = solution[moved.u];
    solution[moved.u] = moved.vj;
    tabu_tenure_table[moved.u][vi] = conflict + iter + pseudoRandNumGen() % 10 + 1;
    ArcNode *temp = adj_yf_list[moved.u].first;

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

        if ((++adj_color_table[adj_vertex][moved.vj]) == 1)
        {
            if (solution[adj_vertex] == moved.vj)
            {
                add_conflict(adj_vertex);
            }
        }
        temp = temp->next;
    }

    if (adj_color_table[moved.u][vi] != 0 && adj_color_table[moved.u][moved.vj] == 0)
        delete_conflict(moved.u);
    if (adj_color_table[moved.u][vi] == 0 && adj_color_table[moved.u][moved.vj] != 0)
        add_conflict(moved.u);
}


void Hybrid_Evolution::make_move()
{
    conflict = min_delta + conflict; // update value of conflict;

    if (conflict < best_conflict)
        best_conflict = conflict; // update minimum conflict of history;

    unsigned int old_color = single_solution[moved.u];
    single_solution[moved.u] = moved.vj;
    tabu_tenure_table[moved.u][old_color] = iter + conflict + pseudoRandNumGen() % 10 + 1; //更新禁忌表

    // update adjacent color table;
    for (int i = 0; i < vertex_edge_num[moved.u]; i++)
    {
        int adj_list_node_moved_i = adj_list[moved.u][i];

        adj_color_table[adj_list_node_moved_i][old_color]--;
        adj_color_table[adj_list_node_moved_i][moved.vj]++;
    }
}


void Hybrid_Evolution::tabu_yf_search(vector<unsigned int> &solution, bool is_limit)
{
    bool is_conflict;
    //cerr <<endl;

    for (int i = 1; i <= num_vertex; i++)
    {
        ArcNode *temp = adj_yf_list[i].first;
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
    // cerr << "initial_f = " << conflict <<endl;
    best_conflict = conflict;
    iter = 0;

    if(is_limit) // set upper bound of iteration;
    {
        while (iter < max_iter && conflict > 0)
        {
            find_yf_move(solution);
            make_yf_move(solution);
            iter++;
        }
    }
    else // not set upper bound of iteration;
    {
        double start_time = clock();

        while (conflict > 0)
        {
            find_yf_move(solution);
            make_yf_move(solution);

            if(iter % 500000 == 0)
            {
                cerr << "Iteration: " << iter << " ";
                double elapsed_time = (clock() - start_time) / CLOCKS_PER_SEC;
                cerr << " elapsed time(s): " << elapsed_time
                     << " frequency:" << double (iter) / elapsed_time << endl;
            }

            iter++;
        }
    }
}


void Hybrid_Evolution::tabu_search()
{
    // compute initial conflict;
    for (int i = 1; i <= num_vertex; i++)
    {
        int num_edge = vertex_edge_num[i];
        unsigned int this_vertex_color = single_solution[i];

        for (int j = 0; j < num_edge; j++)
        {
            unsigned int adj_color = single_solution[adj_list[i][j]];

            if (this_vertex_color == adj_color)
                conflict++;

            adj_color_table[i][adj_color]++; // initialize adjacent color table;
        }
    }

    conflict = conflict / 2;
    best_conflict = conflict;
    cerr << "initial number of confilcts:" << conflict << endl;

    iter = 0;
    start_time = clock();

    while (conflict > 0)
    {
        iter++;
        // cerr << "iter: " << iter << endl;

        if(iter % 1000000 == 0)
        {
            cerr << "Iteration: " << iter << " ";
            double elapsed_time = (clock() - start_time) / CLOCKS_PER_SEC;
            cerr << " elapsed time(s): " << elapsed_time
                 << " frequency:" << double (iter) / elapsed_time << endl;
        }

        find_move();
        make_move();
    }

    end_time = clock();

    double elapsed_time = (end_time - start_time) / CLOCKS_PER_SEC;
    cerr << "success, iterations: " << iter << " elapsed_time(s): " << elapsed_time
         << " frequency:" << double (iter) / elapsed_time << endl;

    // debug function:
    cerr << "max equal nontabu count: " << get_max_equal_nontabu_count() << endl;
    cerr << "max equal tabu count: " << get_max_equal_tabu_count() << endl;
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


void Hybrid_Evolution::cross_over(unsigned int p1, unsigned int p2, vector<unsigned int>& index1)
{
    int A, B;
    Population_solution s[2] = {Population_solution(num_color), Population_solution(num_color)};
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

        for (int j = 0; j < num_color; j++)
        {
            if (s[A].color_num[j] > max_num)
            {
                max_index = j;
                max_num = s[A].color_num[j];
            }
        }

        int num = s[A].color_num[max_index];
        for (int j = 0; j<num; j++)
        {
            int point = s[A].psol[max_index][j];
            index1[point] = i; //只需要保存哪个点分配了哪种颜色，因为马上要对它进行禁忌搜索，其它的保存了又会变

            unsigned int color = s[B].index1s[point];//在B中删除这个点
            unsigned int index2 = s[B].index2s[point];

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
            unsigned int color = pseudoRandNumGen() % num_color;//随机分配到某一种颜色中去
            index1[point] = color;
        }
    }
}

// debug function: compute conflict of a solution
int Hybrid_Evolution::compute_yf_conflict(vector<unsigned int> &solution)
{
    int this_conflict = 0;
    //cerr <<endl;

    for (int i = 1; i <= num_vertex; i++)
    {
        ArcNode *temp = adj_yf_list[i].first;
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

// debug function: compute conflict of a solution
int Hybrid_Evolution::compute_conflict(vector<unsigned int> &solution)
{
    int this_conflict = 0;
    //cerr <<endl;

    for (int i = 1; i <= num_vertex; i++)
    {
        int num_edge = vertex_edge_num[i];
        unsigned int this_vertex_color = solution[i];

        for (int j = 0; j < num_edge; j++)
        {
            unsigned int adj_color = solution[adj_list[i][j]];

            if (this_vertex_color == adj_color)
                this_conflict++;
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


void Hybrid_Evolution::print_adj_yf_list()
{
    cerr << "YFR Adjacency list of graph: " << num_vertex << " " << num_color << endl;
    for(int i=1;i<=num_vertex;i++)
    {
        cerr << "Vertex " << i << ": ";
        ArcNode *temp = adj_yf_list[i].first;
        while (temp)
        {
            cerr << temp->adj_vertex << " ";
            temp = temp->next;
        }
        cerr << endl;
    }
}


// debug function: print adjacency list of graph;
void Hybrid_Evolution::print_adj_list() const
{
    cerr << "Adjacency list of graph: " << num_vertex << " " << num_color << endl;
    for (int i = 1; i <= num_vertex; i++)
    {
        cerr << "Vertex " << i << ": ";
        for (int j = 0;j < vertex_edge_num[i];j++)
        {
            cerr << adj_list[i][j] << " ";
        }
        cerr << endl;
    }
}


// debug function:
int Hybrid_Evolution::get_max_equal_nontabu_count() const
{
    return max_equal_nontabu_count;
}

// debug function:
int Hybrid_Evolution::get_max_equal_tabu_count() const
{
    return max_equal_tabu_count;
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

    memset(&test.adj_yf_list[0], 0, sizeof(test.adj_yf_list[0]) * test.adj_yf_list.size());

    int v1; // first vertex of an edge;
    int v2; // second vertex of an edge;
    while (!feof(fp))
    {
        fscanf(fp, "%d %d\n", &v1, &v2);
        test.insert_adj_yf_list(v1+1, v2+1);
        test.insert_adj_yf_list(v2+1, v1+1);

        test.insert_adj_list(v1+1, v2+1);
    }
    test.print_adj_yf_list();
    test.print_adj_list();

    ///* to debug
    vector<unsigned int> temp_solution_1;
    temp_solution_1.resize(test.num_vertex+1, 0);

    // initialization: set random solution to each solution in the population;
    for (int i = 1; i <= test.num_vertex; i++)
    {
        temp_solution_1[i] = pseudoRandNumGen() % test.num_color;
        //cerr << solution[i] <<' ';
    }

    vector<unsigned int> temp_solution_2 = temp_solution_1;
    for(int i=1;i<=test.single_solution.size();i++)
    {
        test.single_solution[i] = temp_solution_2[i];
    }

    cerr << "YF Conflict before tabu search is: " << test.compute_yf_conflict(temp_solution_1) << endl;
    cerr << "Conflict before tabu search is: " << test.compute_conflict(test.single_solution) << endl;
    // cerr << "iterations: " << test.get_iteration() << endl;

    double start_time = clock();

    // test.tabu_yf_search(temp_solution_1, false);
    test.tabu_search();

    double end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    // cerr << "elapsed time(s): " << elapsed_time << endl;

    cerr << "Iterations: " << test.get_iteration() << " elapsed_time(s): " << elapsed_time
         << " frequency:" << double (test.get_iteration()) / elapsed_time << endl;

    cerr << "Conflict after tabu search is: " << test.compute_yf_conflict(test.single_solution) << endl;
    // cerr << "iterations: " << test.get_iteration() << endl;
     //*/// to debug


    /* to reduce
    // this is also the process of initialization;
    for (int p = 0; p < num_population; p++)
    {
        for(auto& x : test.adj_color_table) memset(&x[0],0,sizeof(int)*x.size());
        for(auto& x : test.tabu_tenure_table) memset(&x[0],0,sizeof(int)*x.size());

        test.conflict = 0;
        test.best_conflict = 0;
        test.conflict_num = 0;

        // initialization: set random solution to each solution in the population;
        for (int i = 1; i <= test.num_vertex; i++)
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

    for(auto & i : test.population_solution)
    {
        for(auto& x : i.psol) memset(&x[0],0,sizeof(int)*x.size());
        memset(&i.color_num[0], 0, sizeof(i.color_num[0]) * i.color_num.size());
        memset(&i.index1s[0], 0, sizeof(i.index1s[0]) * i.index1s.size());
        memset(&i.index2s[0], 0, sizeof(i.index2s[0]) * i.index2s.size());
    }

    double start_time = clock();

    for (int p = 0; p < num_population; p++)
    {
        for (int i = 1; i <= test.num_vertex; i++)
        {
            test.population_solution[p].index1s[i] = test.solution_collection[p][i]; // color of population p, vertex i;
            unsigned int color = test.solution_collection[p][i]; // color of population p, vertex i;
            int color_num = test.population_solution[p].color_num[color];

            test.population_solution[p].psol[color][color_num] = i;
            test.population_solution[p].index2s[i] = color_num++;
            test.population_solution[p].color_num[color] = color_num;
        }
    }


    Population_solution temps(input_num_color);

    long long int population_iteration = 0;
    while (population.min_conflict != 0)
    {
        // random select two index from population as parents;
        unsigned int p1 = pseudoRandNumGen() % num_population, p2;

        do
        {
            p2 = pseudoRandNumGen() % num_population;
        } while (p1 == p2);

        for(auto& x : temps.psol) memset(&x[0],0,sizeof(int)*x.size());
        memset(&temps.color_num[0], 0, sizeof(temps.color_num[0]) * temps.color_num.size());
        memset(&temps.index1s[0], 0, sizeof(temps.index1s[0]) * temps.index1s.size());
        memset(&temps.index2s[0], 0, sizeof(temps.index2s[0]) * temps.index2s.size());
        // cerr << "After 2: " << temps.color_num[17] << endl; // debug memset sentence;

        test.cross_over(p1, p2, temps.index1s);

        // reset adj_color_table and tabu_tenure_table to zero;
        for(auto& x : test.adj_color_table) memset(&x[0],0,sizeof(int)*x.size());
        for(auto& x : test.tabu_tenure_table) memset(&x[0],0,sizeof(int)*x.size());

        test.conflict = 0;
        test.best_conflict = 0;
        test.conflict_num = 0;

        test.tabu_search(temps.index1s, true); // 仅仅需要对新形成的temps进行禁忌搜索;

        for (int i = 1; i <= test.num_vertex; i++)
        {//变成划分的形式
            unsigned int color = temps.index1s[i];
            int color_num = temps.color_num[color];
            temps.psol[color][color_num] = i;
            temps.index2s[i] = color_num;
            temps.color_num[color] = ++color_num;
        }

        int max_conflict = -1, max_conflict_index;

        for (int i = 0; i < num_population; i++)
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

        if(population_iteration % 10 == 0)
        {
            cerr << "Population iteration: " << population_iteration << endl;
            cerr << "min conflict: " << population.min_conflict << endl;
            cerr << "min conflict index: " << population.min_conflict_index << endl;
        }

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
        for(int i=1;i<=test.num_vertex;i++)
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
        *///  to reduce


    return 0;
}

/*
 * debugging command:
 * laptop: C:\wamp64\www\npbenchmark\npbenchmark-main\yuanFang_reduce
 * home server: /home/ssfc/yuanFang_reduce
 * g++ hea.cpp -g; gdb a.out
 * r 11 6 chvatal.txt
 * r 11 6 DSJC0250.9.txt
 * r 11 6 DSJC1000.9.txt
 */