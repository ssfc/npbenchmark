//
// Created by take_ on 2023/8/22.
//
#include "low_level.h"

#include <utility>

using namespace std;

LowLevel::LowLevel(vector<vector<int>> input_map,
                   vector<Agent> input_agents,
                   vector<unordered_map<Coordinate, int>> input_heuristics,
                   int input_agent_id,
                   vector<Constraint> input_constraints):
          map(std::move(input_map)),
          agents(std::move(input_agents)),
          heuristics(std::move(input_heuristics)),
          agent_id(input_agent_id),
          constraints(std::move(input_constraints)),
          num_generated(0),
          num_expanded(0),
          max_constraints(0)
{
    num_rows = map.size();
    num_columns = map[0].size();
    num_agents = agents.size();
}

unordered_map<int, vector<Constraint>> LowLevel::build_constraint_table(int input_agent_id)
{
    unordered_map<int, vector<Constraint>> this_constraint_table;
    if(constraints.empty())
    {
        return this_constraint_table;
    }

    return this_constraint_table;
}


vector<Path> LowLevel::low_level_search()
{
    cerr << "> build constraint table" << endl;
    auto constraint_table_i = build_constraint_table(agent_id);

    // print constraint table
    cerr << "constraint table: " << agent_id << " {";
    for (const auto& pair : constraint_table_i)
    {
        for(const auto& element : pair.second)
        {
            if(element.locations.size() == 1)
            {
                cerr << "[agent " << element.agent_id << " location " << element.locations[0].x << ",";
                cerr << element.locations[0].y << " " << "time step " << element.time_step << "], ";
            }
            else if(element.locations.size() == 2)
            {
                cerr << "[agent " << element.agent_id << " location (" << element.locations[0].x << ",";
                cerr << element.locations[0].y << ")(" << element.locations[1].x << "," << element.locations[1].y
                << ") time step " << element.time_step << "]";
            }
        }
    }
    cerr << "}" << endl;

    constraint_table = constraint_table_i;
    if(!constraint_table_i.empty())
    {
        // self.max_constraints[i] = max(constraint_table_i.keys())
    }


    vector<Path> paths;

    return paths;
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













