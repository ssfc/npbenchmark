//
// Created by take_ on 2023/8/22.
//
#include "low_level.h"

#include <utility>

using namespace std;

LowLevel::LowLevel(std::vector<std::vector<int>> input_map):
          map(std::move(input_map))
{
    num_rows = map.size();
    num_columns = map[0].size();
}

void LowLevel::print_map()
{
    cerr << "num rows: " << num_rows << " num columns: " << num_columns << endl;
    for(auto & each_row : map)
    {
        for(int element : each_row)
        {
            cerr << element << " ";
        }
        cerr << endl;
    }

    /*
    cerr << "num agents: " << num_agents << endl;
    for(auto agent : agents)
    {
        cerr << "(" << agent.start.x << "," << agent.start.y << ")->(";
        cerr << agent.goal.x << "," << agent.goal.y << ")" << endl;
    }
     */
}













