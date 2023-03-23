//
// Created by take_ on 2023/3/22.
//

#ifndef SDK_VRPTW2D_ANTCOLONY_H
#define SDK_VRPTW2D_ANTCOLONY_H
#include "VRPTW2d.h"
#include <array>
#include <random>
#include <vector>

using namespace szx;


class AntColony
{
private: // member variables.
    int num_nodes; // 客户数量+1, 因为算上了仓库; 101
    int num_agents; // agent数量
    int max_num_agents; // 最大agents数量;
    int capacity; // 车辆容量

    // Dimension: num__nodes == 客户数量+1
    // Meaning: 存储客户数据, 就是算例中一行一行的东西;
    std::vector<Node2d> nodes;

    // debug variables:
    std::mt19937 generated_random;

public: // member functions.
    AntColony(VRPTW2d& input, int input_seed);
    ~AntColony();

    // debug functions;
    void init_rand(int seed) { generated_random = std::mt19937(seed); }
};



#endif //SDK_VRPTW2D_ANTCOLONY_H
