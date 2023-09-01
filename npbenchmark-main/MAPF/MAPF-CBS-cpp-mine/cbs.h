//
// Created by take_ on 2023/8/2.
//

#ifndef DRAW_PATH_PY_CBS_H

#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include "low_level.h"
#include "util.h"


struct Collision
{
    int agent_id_1; // first collision agent;
    int agent_id_2; // second collision agent;
    // collision location, vertex collision contains 1 location, edge collision contains 2 locations.
    std::vector<Coordinate> location;
    int time_step; // collision time
};

// Node of constraint tree
struct CBSNode
{
    int cost;
    std::vector<Constraint> constraints;
    std::vector<Path> paths;
    std::vector<Collision> collisions;
};

struct OpenQueueNode
{
    int cost;
    Coordinate location;

    bool operator<(const OpenQueueNode& other) const
    {
        if (cost != other.cost)
        {
            return cost > other.cost; // 把数值小的放在堆顶
        }
        else
        {
            if (location.x != other.location.x)
            {
                return location.x > other.location.x; // 把数值小的放在堆顶
            }
            else
            {
                return location.y > other.location.y; // 把数值小的放在堆顶
            }
        }
    }
};


class CBS
{
private:
    size_t num_rows;
    size_t num_columns;
    std::vector<std::vector<int>> map;
    int num_agents;
    std::vector<Agent> agents;
    std::vector<CBSNode> open_list;
    std::vector<std::unordered_map<Coordinate, int>> heuristics;
    std::vector<Path> paths;

    // debug var;
    int iter;
    double start_time;
public:
    CBS(); // constructor
    static Coordinate move_agent(Coordinate current_location, int direction);
    std::unordered_map<Coordinate, int> compute_heuristics(Coordinate input_goal); // 计算终点和每个点的距离
    void find_solution();

    // debug func
    std::vector<std::vector<int>> get_map();
    std::vector<Agent> get_agents();
    void print_map();
    void print_heuristics();
};

#define DRAW_PATH_PY_CBS_H

#endif //DRAW_PATH_PY_CBS_H
