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

void CBS::find_solution()
{

}

void CBS::print_map()
{
    cerr << num_rows << " " << num_columns << endl;
    for(auto & each_row : map)
    {
        for(int element : each_row)
        {
            cerr << element << " ";
        }
        cerr << endl;
    }
    cerr << num_agents << endl;
    for(auto agent : agents)
    {
        cerr << "(" << agent.start.x << "," << agent.start.y << ")->(";
        cerr << agent.goal.x << "," << agent.goal.y << ")" << endl;
    }
}


