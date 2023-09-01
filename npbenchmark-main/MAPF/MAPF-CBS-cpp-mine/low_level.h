//
// Created by take_ on 2023/8/22.
//

#ifndef DRAW_PATH_PY_LOW_LEVEL_H
#define DRAW_PATH_PY_LOW_LEVEL_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "util.h"

class LowLevel
{
    int num_rows;
    int num_columns;
    std::vector<std::vector<int>> map;
    int num_agents;
    std::vector<Agent> agents;
    std::vector<std::unordered_map<Coordinate, int>> heuristics;

public:
    LowLevel(std::vector<std::vector<int>> input_map,
             std::vector<Agent> input_agents); // constructor

    // debug func
    void print_map();
};



#endif //DRAW_PATH_PY_LOW_LEVEL_H
