//
// Created by take_ on 2023/8/22.
//

#ifndef DRAW_PATH_PY_LOW_LEVEL_H
#define DRAW_PATH_PY_LOW_LEVEL_H

#include <iostream>
#include <tuple>
#include <unordered_map>
#include <vector>
#include "util.h"

struct LowLevelOpenNode
{
    int f_value;
    int h_value;
    Coordinate location;
    int num_generate;
};

class LowLevel
{
    size_t num_rows;
    size_t num_columns;
    std::vector<std::vector<int>> map;
    size_t num_agents;
    std::vector<Agent> agents;
    std::vector<std::unordered_map<Coordinate, int>> heuristics;
    int agent_id;
    std::vector<Constraint> constraints;
    std::unordered_map<int, std::vector<Constraint>> constraint_table;
    int num_generated;
    int num_expanded;
    int max_constraints;
    std::vector<LowLevelOpenNode> open_list;
    std::unordered_map<LocationTime, std::vector<LowLevelOpenNode>> closed_list;

public:
    LowLevel(std::vector<std::vector<int>> input_map,
             std::vector<Agent> input_agents, // constructor
             std::vector<std::unordered_map<Coordinate, int>> input_heuristics,
             int input_agent_id,
             std::vector<Constraint> input_constraints);

    std::unordered_map<int, std::vector<Constraint>> build_constraint_table(int input_agent_id);
    std::vector<Path> low_level_search();

    // debug func
    void print_map();
};



#endif //DRAW_PATH_PY_LOW_LEVEL_H
