//
// Created by take_ on 2023/8/2.
//

#include "cbs.h"

using namespace std;

CBS::CBS()
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

        map_file.close();
    }
    else
    {
        cerr << "Error opening file." << endl;
    }

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
}


