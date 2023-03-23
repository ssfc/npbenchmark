//
// Created by take_ on 2023/3/22.
//

#ifndef SDK_VRPTW2D_ANTCOLONY_H
#define SDK_VRPTW2D_ANTCOLONY_H
#include <array>
#include <random>
#include <vector>

using NodeId = int;
using VehicleId = int;
using Coord = double;
using Capacity = int;
using Time = int;
using Coord2d = std::array<Coord, 2>;
// this struct corresponds to Node.java;
struct Node2d {
    Coord2d coords; // double
    Capacity demand; // int
    Time minStayTime; // int
    Time windowBegin; // int
    Time windowEnd; // int
};

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
    AntColony(int input_num_nodes, int input_num_agents, int input_capacity,
               int input_seed);
    ~AntColony();

    // debug functions;
    void init_rand(int seed) { generated_random = std::mt19937(seed); }
};



#endif //SDK_VRPTW2D_ANTCOLONY_H
