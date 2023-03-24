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
                     untreated(input.nodeNum, 0), // 1d array
                     iter(0),
                     max_iter(50),
                     solutions(input.nodeNum), // array
                     agent_position(input.nodeNum - 1), // 1d array
                     pheromone(num_nodes, vector<double>(num_nodes, 0.0)), // 2d array
                     heuristic(num_nodes, vector<double>(num_nodes, 0.0)), // 2d array
                     init_pheromone(0)
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