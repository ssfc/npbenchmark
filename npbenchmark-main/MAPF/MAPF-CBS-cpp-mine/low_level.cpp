//
// Created by take_ on 2023/8/22.
//
#include "low_level.h"

#include <utility>

using namespace std;

LowLevel::LowLevel(std::vector<std::vector<int>> input_map,
                   std::vector<Agent> input_agents,
                   std::vector<std::unordered_map<Coordinate, int>> input_heuristics,
                   int input_agent_id,
                   std::vector<Constraint> input_constraints):
          map(std::move(input_map)),
          agents(std::move(input_agents)),
          heuristics(std::move(input_heuristics)),
          agent_id(input_agent_id),
          constraints(std::move(input_constraints))
{
    num_rows = int (map.size());
    num_columns = int (map[0].size());
    num_agents = int (agents.size());
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

    cerr << "num agents: " << num_agents << endl;
    for(auto agent : agents)
    {
        cerr << "(" << agent.start.x << "," << agent.start.y << ")->(";
        cerr << agent.goal.x << "," << agent.goal.y << ")" << endl;
    }
}













