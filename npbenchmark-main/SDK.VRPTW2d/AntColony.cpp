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
                     w1(0.9),
                     w2(0.1),
                     alpha(0.05),
                     beta(1.0),
                     theta(5.0),
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

    cerr << "total distance: " << total_distance << endl;

    int num_paths = num_nodes * (num_nodes - 1); // 每个node和除自己外的其他node构建路径
    cerr << "num_path: " << num_paths << endl;

    init_pheromone = double (num_paths) / (total_distance * num_nodes);
    cerr << "init pheromone: " << init_pheromone << endl;

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

    cerr << "pheromone[2][1]: " << pheromone[2][1] << endl;
    cerr << "heuristic[2][1]: " << heuristic[2][1] << endl;
}


void AntColony::reset()
{
    // 初始化每位agent未服务的客户
    for (int i = 0; i < num_agents; i++)
    {
        untreated[i].clear();
        for ( int j = 1; j < num_nodes; j++) // 之所以从1开始算, 是因为0是仓库, 1开始才是客户;
        {
            untreated[i].push_back(j);
        }
    }

    // evaluate initial untreated;
    // for (int i = 0; i < num_agents; i++)
    // {
    //     print_vector(to_string(i), untreated[i]);
    // }

    // 初始化起始服务客户
    for (int i = 0; i < num_agents; i++)
    {
        solutions[i] = Solution();
        agent_position[i] = 0; // 所有车辆的起始位置都是仓库;
    }
}


int AntColony::select_next(int k, Route route)
{
    // 若车辆k没有尚未访问的客户，返回仓库
    if (untreated[k].empty())
    {
        cerr << "untreated[k] empty!";
        return 0;
    }

    // 计算概率
    double sum_pheromone = 0;
    double sum_time = 0;
    vector<double> info_pheromone(num_agents, 0);
    vector<double> info_time(num_agents, 0);
    for (int i = 0; i < untreated[k].size(); i++)
    {
        info_pheromone[i] = pow(double (pheromone[agent_position[k]][untreated[k][i]]), beta)
                           * pow(double (heuristic[agent_position[k]][untreated[k][i]]), theta);
        // cerr << "info pheromone: " << info_pheromone[i] << endl;
        info_time[i] = 1.0 / (abs(route.time - nodes[untreated[k][i]].window_begin) +
                            abs(route.time - nodes[untreated[k][i]].window_end));
        sum_pheromone += info_pheromone[i];
        sum_time += info_time[i];
    }
    cerr << "sum pheromone: " << sum_pheromone << endl;
    cerr << "sum time: " << sum_time << endl;

    double rate = generated_random() / double(std::mt19937::max());
    rate = 0.7;
    int next = 0;
    double sum_prob = 0; // Ah... ChatGPT also recommends this name;

    // 生成0-1随机数，累加概率，若大于当前累加部分，返回当前城市编号
    for (int i = 0; i < untreated[k].size(); i++)
    {
        sum_prob += info_pheromone[i] * w1 / sum_pheromone + info_time[i] * w2 / sum_time;
        if (rate < sum_prob)
        {
            next = untreated[k][i];
            // 检验合法性
            double time = route.time + travel_times[agent_position[k]][next];
            double load = route.load + nodes[next].demand;
            if (time <= nodes[next].window_end && load <= capacity)
            {
                break;
            }
        }
    }
    // 检验合法性
    double time = route.time + travel_times[agent_position[k]][next];
    double load = route.load + nodes[next].demand;
    if (time > nodes[next].window_end || load > capacity)
    {
        next = 0;
    }

    return next;
}


void AntColony::construct_solution()
{
    // 为每一位agent分别构造解
    // for (int i = 0; i < num_agents; i++)
    for (int i = 0; i < 1; i++) // debug one iter;
    {
        // 路径开始
        Route route = Route();
        route.route.push_back(0); // 先把仓库加进去;
        print_vector("init route", route.route);

        int debug_counter = 0;
        // while(!untreated[i].empty()) // 车辆i还有没有访问的客户
        while(!untreated[i].empty() && debug_counter < 2) // debug first two iterations;
        {
            cerr << "debug counter: " << debug_counter << endl;
            int next = select_next(i, route);
            cerr << "next: " << next << endl;

            // 如果下一个选择不合法或客户已配送完毕
            if (next == 0)
            {
                route.route.push_back(0);
                route.time += travel_times[agent_position[i]][0];
                route.distance += travel_times[agent_position[i]][0];
                solutions[i].Routes.push_back(route);
                solutions[i].total_cost += route.distance;
                route = Route();
                route.route.push_back(0);
                agent_position[i] = 0;
            }
            else
            {
                route.route.push_back(next);
                route.load += nodes[next].demand;
                route.time = max(route.time + travel_times[agent_position[i]][next], nodes[next].window_begin) + nodes[next].min_stay_time;
                route.distance += travel_times[agent_position[i]][next];
                agent_position[i] = next;
                for (int j = 0; j < untreated[i].size(); j++)
                {
                    if (untreated[i][j] == next)
                    {
                        untreated[i].erase(untreated[i].begin() + j);
                    }
                }
            }

            debug_counter++;
        }

        // 最后一条路径返回配送中心
        route.route.push_back(0);
        route.time = max(travel_times[agent_position[i]][0], nodes[0].window_begin) + nodes[0].min_stay_time;
        route.distance += travel_times[agent_position[i]][0];
        solutions[i].Routes.push_back(route);
        solutions[i].total_cost += route.distance;
    }
}


void AntColony::ACS_Strategy()
{
    double begin_time = clock();
    best_solution.total_cost = INT_MAX;
    init_other();

    while (iter < max_iter && iter < 1)
    {
        reset();//初始化agent信息
        cerr << "reset done" << endl;
        construct_solution();//对于所有的agent构造一个完整的tour

        /*
        update_pheromone();//更新信息素
        if(iter % 5 == 0)
        {
            double elapsed_time= (System.nanoTime() - begin_time)/(1e9); // 因为是纳秒, 所以除以1e9换算;
            System.out.println("iter: " + iter + "\tnum agents: " + best_solution.routes.size()
                               + "\tbest solution cost: " + df.format(best_solution.total_cost)
                               + "\telapsed time(s): " + df.format(elapsed_time)
                               + "\tfrequency: " + df.format((double) iter / elapsed_time));
        }

         */
        iter++;
    }



    double elapsed_time = (clock() - begin_time)/ CLOCKS_PER_SEC;
    cerr << "success, iterations: " << iter << "\tnum agents: " << best_solution.Routes.size()
    << "\telapsed time(s): " << elapsed_time << "\tfrequency: " << iter / elapsed_time;
}

// debug function:
void AntColony::print_vector(const string& name, vector<int> &vect)
{
    cerr << name << ": ";
    for(int i : vect)
    {
        cerr << i << " ";
    }
    cerr << endl;
}