//
// Created by take_ on 2023/8/2.
//

#include "cbs.h"

using namespace std;

CBS::CBS():
    num_rows(0),
    num_columns(0),
    num_agents(0),
    iter(0),
    start_time(clock())
{
    ifstream map_file("test_1.txt");
    if (map_file.is_open())
    {
        map_file >> num_rows >> num_columns;

        map.resize(num_rows);
        for(int i=0;i<num_rows;i++)
        {
            map[i].resize(num_columns);
        }

        for (int i = 0; i < num_rows; i++)
        {
            for (int j = 0; j < num_columns; j++)
            {
                char c;
                map_file >> c;
                if(c=='@')
                {
                    map[i][j] = 0;
                }
                else if(c=='.')
                {
                    map[i][j] = 1;
                }
            }
        }

        map_file >> num_agents;
        agents.resize(num_agents);
        for(int i=0;i<num_agents;i++)
        {
            map_file >> agents[i].start.x;
            map_file >> agents[i].start.y;
            map_file >> agents[i].goal.x;
            map_file >> agents[i].goal.y;
        }

        map_file.close();

        int iter_goal = 0;
        for(auto this_agent : agents)
        {
            cerr << "iter_goal: " << iter_goal << endl;
            auto this_heuristics = compute_heuristics(this_agent.goal);
            heuristics.push_back(this_heuristics);
            print_heuristics();
            iter_goal++;
        }
    }
    else
    {
        cerr << "Error opening file." << endl;
    }

}

Coordinate CBS::move_agent(Coordinate current_location, int direction)
{
    if(direction == 0)
    {
        return current_location;
    }
    else if(direction == 1)
    {
        // east
        return Coordinate{current_location.x + 1, current_location.y};
    }
    else if(direction == 2)
    {
        // west
        return Coordinate{current_location.x - 1, current_location.y};
    }
    else if(direction == 3)
    {
        // north
        return Coordinate{current_location.x, current_location.y + 1};
    }
    else if(direction == 4)
    {
        // south
        return Coordinate{current_location.x, current_location.y - 1};
    }
    else
    {
        return Coordinate{-1, -1};
    }
}

unordered_map<Coordinate, int> CBS::compute_heuristics(Coordinate input_goal)
{
    priority_queue<OpenQueueNode> open_queue;
    unordered_map<Coordinate, int> closed_list;
    auto root = OpenQueueNode{0, input_goal};
    open_queue.push(root);
    closed_list[input_goal] = root.cost;
    int iter_computed = 0;
    while(!open_queue.empty() && iter_computed<3)
    {
        cerr << "iter computed: " << iter_computed << endl;
        auto current = open_queue.top();
        open_queue.pop();
        for(int i=0;i<5;i++)
        {
            auto child_location = move_agent(current.location, i);
            int child_cost = current.cost + 1;
        }

        iter_computed++;
    }


    unordered_map<Coordinate, int> h_value;

    return h_value;
}

void CBS::find_solution()
{
    // A1 LINE 1
    // R.constraints = ∅ // 最开始无约束
    CBSNode root;
    root.cost = 0;
    open_list.emplace_back(root);

    // A1 LINE 2
    // R.solution = find individual paths using the low-level() // 用低层算法计算每个智能体的path
    for(int i=0;i<num_agents;i++)
    {

    }
}

void CBS::find_path()
{

}

void CBS::print_map()
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

void CBS::print_heuristics()
{
    for(const auto& this_heuristic : heuristics)
    {
        for (auto pair : this_heuristic)
        {
            const Coordinate& coord = pair.first;
            cerr << "Key: " << coord.x << " " << coord.y << " , Value: " << pair.second << endl;
        }
        cerr << endl;
    }
}

vector<vector<int>> CBS::get_map()
{
    return map;
}

std::vector<Agent> CBS::get_agents()
{
    return agents;
}




