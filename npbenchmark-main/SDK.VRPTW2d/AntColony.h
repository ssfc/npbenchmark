//
// Created by take_ on 2023/3/22.
//

#ifndef SDK_VRPTW2D_ANTCOLONY_H
#define SDK_VRPTW2D_ANTCOLONY_H

#include "VRPTW2d.h"
#include <array>
#include <climits>
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
    // Meaning: travel_times[i][j], 点i和点j的距离;
    // Comment: 这也是为什么长度选num__nodes而不是num__customers的原因, 因为仓库和各点之间的距离也需要计算;
    std::vector<std::vector<int>> travel_times;

    // Dimension: num__agents == num__customers == num__nodes - 1;
    // untreated[i]: 记录agent i尚未服务过的客户
    std::vector<std::vector<int>> untreated;

    // Dimension: num__nodes == 客户数量 + 1
    // Meaning: agents
    std::vector<Solution> solutions;

    Solution best_solution;

    // Dimension: num__agents == num__nodes - 1
    // agent[i]: 车辆i当前的位置
    std::vector<int> agent_position;

    // Dimension: num__nodes * num__nodes == (客户数量+1) * (客户数量+1)
    // pheromone[i][j]: 点i到点j路径的信息素;
    std::vector<std::vector<double>> pheromone;

    // Dimension: num__nodes * num__nodes == (客户数量+1) * (客户数量+1)
    // heuristic[i][j]: 点i到点j路径的启发值
    std::vector<std::vector<double>> heuristic;

    double init_pheromone; // 信息素初始值

    double w1; // 计算概率的参数
    double w2; // 计算概率的参数
    double alpha; // 计算infoPhe的参数，
    double beta; // 计算infoPhe的参数，
    double theta; // 计算infoPhe的参数，

    int iter;
    int max_iter; // 最大迭代次数

    // debug variables:
    std::mt19937 generated_random;

public: // member functions.
    AntColony(VRPTW2d& input, std::vector<std::vector<Time>>& input_travel_times, int input_seed);
    ~AntColony();
    void init_other();
    void reset();
    void construct_solution();
    void ACS_Strategy();

    // debug functions;
    void init_rand(int seed) { generated_random = std::mt19937(seed); }
    static void print_vector(const std::string& name, std::vector<int> &vect);
};



#endif //SDK_VRPTW2D_ANTCOLONY_H
