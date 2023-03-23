//
// Created by take_ on 2023/3/22.
//

#include "AntColony.h"

#include <iostream>
using namespace std;


AntColony::AntColony(VRPTW2d& input, std::vector<std::vector<Time>>& input_travel_times, int input_seed):
                     num_nodes(input.nodeNum),
                     max_num_agents(input.maxVehicleNum),
                     capacity(input.vehicleCapacity)
{
    cerr << "num nodes: " << num_nodes << endl;
    num_agents = input.nodeNum - 1;
    cerr << "num cars: " << num_agents << endl;
    cerr << "max cars: " << max_num_agents << endl;
    cerr << "capacity: " << capacity << endl;
    init_rand(input_seed); // initialize random generator;

    nodes = input.nodes;

}

AntColony::~AntColony()
= default;