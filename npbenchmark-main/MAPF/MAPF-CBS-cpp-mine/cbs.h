//
// Created by take_ on 2023/8/2.
//

#ifndef DRAW_PATH_PY_CBS_H

#include <fstream>
#include <iostream>
#include <vector>

class CBS
{
private:
    int num_rows;
    int num_columns;
    std::vector<std::vector<int>> map;
    int num_agents;
public:
    CBS(); // constructor

    // debug func
    void print_map();
};

#define DRAW_PATH_PY_CBS_H

#endif //DRAW_PATH_PY_CBS_H
