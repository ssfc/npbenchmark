//
// Created by take_ on 2023/3/22.
//

# include "AntColony.h"

using namespace std;


AntColony::AntColony(int input_num_nodes, int input_num_agents,
                     int input_seed)
{
    init_rand(input_seed); // initialize random generator;


}

AntColony::~AntColony()
= default;