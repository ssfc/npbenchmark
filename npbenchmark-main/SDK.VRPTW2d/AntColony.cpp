//
// Created by take_ on 2023/3/22.
//

#include "AntColony.h"

#include <iostream>
using namespace std;


AntColony::AntColony(int input_num_nodes, int input_num_agents,
                     int input_seed):
                     num_nodes(input_num_nodes),
                     num_agents(input_num_agents)
{
    cerr << "num nodes: " << num_nodes << endl;
    cerr << "max cars: " << num_agents << endl;
    init_rand(input_seed); // initialize random generator;


}

AntColony::~AntColony()
= default;