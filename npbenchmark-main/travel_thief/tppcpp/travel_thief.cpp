//
// Created by take_ on 2023/5/9.
//

#include "travel_thief.h"

#include <iostream>

using namespace std;





void TravelThief::print_node_coords()
{
    cerr << "num node coords: " << node_coords.size() << endl;
    for(int i=0;i<node_coords.size();i++)
    {
        cerr << i << " " << node_coords[i].x << " " << node_coords[i].y << endl;
    }
}

void TravelThief::print_items()
{
    cerr << "num items: " << items.size() << endl;
    for(int i=0;i<items.size();i++)
    {
        cerr << i << " " << items[i].profit << " " << items[i].weight << " " << items[i].assigned_node << endl;
    }
}
