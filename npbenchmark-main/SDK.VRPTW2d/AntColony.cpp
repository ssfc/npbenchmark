//
// Created by take_ on 2023/3/22.
//

#include "AntColony.h"

#include <iostream>
using namespace std;


AntColony::AntColony(VRPTW2d& input, std::vector<std::vector<Time>>& input_travel_times, int input_seed):
                     num_nodes(input.nodeNum),
                     num_agents(input.nodeNum - 1),
                     max_num_agents(input.maxVehicleNum),
                     capacity(input.vehicleCapacity),
                     nodes(input.nodes), // 1d array
                     travel_times(input_travel_times), // 2d array
                     untreated(input.nodeNum), // 2d array
                     solutions(input.nodeNum), // array
                     agent_position(input.nodeNum - 1), // 1d array
                     pheromone(num_nodes, vector<double>(num_nodes, 0.0)), // 2d array
                     heuristic(num_nodes, vector<double>(num_nodes, 0.0)), // 2d array
                     init_pheromone(0.0),
                     w1(0.0),
                     w2(0.0),
                     alpha(0.0),
                     beta(0.0),
                     theta(0.0),
                     iter(0),
                     max_iter(50)
{
    init_rand(input_seed); // initialize random generator;
    cerr << "num nodes: " << num_nodes << endl;
    cerr << "num cars: " << num_agents << endl;
    cerr << "max cars: " << max_num_agents << endl;
    cerr << "capacity: " << capacity << endl;

    cerr << "travel times[1][2]: " << travel_times[1][2] << endl;
    cerr << "travel times[1][1]: " << travel_times[1][1] << endl;
    cerr << "travel times[0][0]: " << travel_times[0][0] << endl;
}

AntColony::~AntColony()
= default;

void AntColony::init_other()
{
    // 计算信息素初始值
    double total_distance = 0;
    for (int i = 0; i < num_nodes; i++)
    {
        for (int j = 0; j < num_nodes; j++)
        {
            if (i != j)
            {
                total_distance += travel_times[i][j];
            }
        }
    }

    double num_paths = num_nodes * (num_nodes - 1); // 每个node和除自己外的其他node构建路径
    cerr << "num: " << num_paths << endl;

    init_pheromone = num_paths / (total_distance * num_nodes);

    // 初始化信息素、启发值
    for (int i = 0; i < num_nodes; i++)
    {
        for (int j = 0; j < num_nodes; j++)
        {
            if (i != j)
            {
                pheromone[i][j] = init_pheromone;
                pheromone[j][i] = init_pheromone;
                heuristic[i][j] = 1 / (double) travel_times[i][j];
                heuristic[j][i] = 1 / (double) travel_times[i][j];
            }
        }
    }
}


void AntColony::ACS_Strategy()
{
    double begin_time = clock();
    best_solution.total_cost = INT_MAX;
    init_other();

    /*
    while (iter < max_iter)
    {
        reset();//初始化agent信息
        construct_solution();//对于所有的agent构造一个完整的tour
        update_pheromone();//更新信息素
        if(iter % 5 == 0)
        {
            double elapsed_time= (System.nanoTime() - begin_time)/(1e9); // 因为是纳秒, 所以除以1e9换算;
            System.out.println("iter: " + iter + "\tnum agents: " + best_solution.routes.size()
                               + "\tbest solution cost: " + df.format(best_solution.total_cost)
                               + "\telapsed time(s): " + df.format(elapsed_time)
                               + "\tfrequency: " + df.format((double) iter / elapsed_time));
        }

        iter++;
    }

     */

    double elapsed_time = (clock() - begin_time)/ CLOCKS_PER_SEC;
    cerr << "success, iterations: " << iter << "\tnum agents: " << best_solution.Routes.size()
    << "\telapsed time(s): " << elapsed_time << "\tfrequency: " << iter / elapsed_time;
}