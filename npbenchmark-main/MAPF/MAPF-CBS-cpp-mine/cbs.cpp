//
// Created by take_ on 2023/8/2.
//

#include "cbs.h"

using namespace std;

CBS::CBS():
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
    }
    else
    {
        cerr << "Error opening file." << endl;
    }

}

int CBS::compute_heuristics(Coordinate input_goal)
{
    priority_queue<OpenQueueNode> open_queue;
    unordered_map<Coordinate, int> closed_list;
    auto root = OpenQueueNode{0, input_goal};
    open_queue.push(root);
    closed_list[input_goal] = root.cost;
    while(!open_queue.empty())
    {
        auto current = open_queue.top();
        open_queue.pop();
    }


    int h_value = 0;

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

vector<vector<int>> CBS::get_map()
{
    return map;
}

std::vector<Agent> CBS::get_agents()
{
    return agents;
}




