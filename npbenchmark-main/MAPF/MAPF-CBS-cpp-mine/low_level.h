//
// Created by take_ on 2023/8/22.
//

#ifndef DRAW_PATH_PY_LOW_LEVEL_H
#define DRAW_PATH_PY_LOW_LEVEL_H

#include <iostream>
#include <vector>

class LowLevel
{
    int num_rows;
    int num_columns;
    std::vector<std::vector<int>> map;


public:
    LowLevel(std::vector<std::vector<int>> input_map); // constructor

    // debug func
    void print_map();
};



#endif //DRAW_PATH_PY_LOW_LEVEL_H
