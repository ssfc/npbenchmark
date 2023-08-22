//
// Created by take_ on 2023/8/2.
//

#ifndef DRAW_PATH_PY_CBS_H

#include <fstream>
#include <iostream>
#include <vector>
#include "util.h"

struct Agent
{
    Coordinate start;
    Coordinate goal;
};

struct Constraint
{
    int agent_id; // 人物
    Coordinate location; // 地点
    int timestep; // 时间
};

struct Path
{
    std::vector<Coordinate> nodes;
};

struct Collision
{
    int agent_id_1; // first collision agent;
    int agent_id_2; // second collision agent;
    // collision location, vertex collision contains 1 location, edge collision contains 2 locations.
    std::vector<Coordinate> location;
    int timestep; // collision time
};

// Node of constraint tree
struct CBSNode
{
    int cost;
    std::vector<Constraint> constraints;
    std::vector<Path> paths;
    std::vector<Collision> collisions;
};


class CBS
{
private:
    int num_rows;
    int num_columns;
    std::vector<std::vector<int>> map;
    int num_agents;
    std::vector<Agent> agents;
    std::vector<CBSNode> open_list;

    // debug var;
    int iter;
    double start_time;
public:
    CBS(); // constructor
    void find_solution();
    void find_path();

    // debug func
    void print_map();
};

#define DRAW_PATH_PY_CBS_H

#endif //DRAW_PATH_PY_CBS_H
