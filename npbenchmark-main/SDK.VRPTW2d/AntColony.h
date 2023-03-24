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

    // Dimension: num__nodes * num__nodes == (客户数量+1) * (客户数量+1)
    // Meaning: distance[i][j], 点i和点j的距离;
    // Comment: 这也是为什么长度选num__nodes而不是num__customers的原因, 因为仓库和各点之间的距离也需要计算;
    std::vector<std::vector<int>> travel_times;

    // Dimension: num__agents == num__customers == num__nodes - 1;
    // untreated[i]: 记录agent i尚未服务过的客户
    std::vector<int> untreated;

    int iter;
    int max_iter; // 最大迭代次数

    // Dimension: num__nodes == 客户数量 + 1
    // Meaning: agents
    std::vector<Solution> solutions;

    // debug variables:
    std::mt19937 generated_random;

public: // member functions.
    AntColony(VRPTW2d& input, std::vector<std::vector<Time>>& input_travel_times, int input_seed);
    ~AntColony();

    // debug functions;
    void init_rand(int seed) { generated_random = std::mt19937(seed); }
};



#endif //SDK_VRPTW2D_ANTCOLONY_H
