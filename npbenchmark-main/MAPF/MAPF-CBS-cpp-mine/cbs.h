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

class CBS
{
private:
    int num_rows;
    int num_columns;
    std::vector<std::vector<int>> map;
    int num_agents;
    std::vector<Agent> agents;

    // debug var;
    int iter;
    double start_time;
public:
    CBS(); // constructor

    // debug func
    void print_map();
};

#define DRAW_PATH_PY_CBS_H

#endif //DRAW_PATH_PY_CBS_H
