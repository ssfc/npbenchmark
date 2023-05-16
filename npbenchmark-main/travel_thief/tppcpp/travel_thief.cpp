//
// Created by take_ on 2023/5/9.
//

#include "travel_thief.h"
#include <cmath>
#include <iostream>

using namespace std;

TravelThief::TravelThief():
dimension(0),
num_items(0),
capacity_of_knapsack(0),
min_speed(0.0),
max_speed(0.0),
renting_ratio(0.0)
{

}

TravelThief::~TravelThief()
= default;

void TravelThief::print_node_coords()
{
    cerr << "num node coords: " << node_coords.size() << endl;
    for(int i=0;i<node_coords.size();i++)
    {
        cerr << i << " " << node_coords[i].x << " " << node_coords[i].y << endl;
    }
}

void TravelThief::compute_distances()
{
    distances.resize(node_coords.size());
    for(int i=0;i<node_coords.size();i++)
    {
        distances[i].resize(node_coords.size(), 0.0);
    }

    for(int i=0;i<node_coords.size();i++)
    {
        for(int j=0;j<node_coords.size();j++)
        {
            distances[i][j] = distances[j][i] = sqrt(
                    (node_coords[i].x - node_coords[j].x) * (node_coords[i].x - node_coords[j].x)
                    + (node_coords[i].y - node_coords[j].y) * (node_coords[i].y - node_coords[j].y));
        }
    }

    cerr << "distance 0 and 1: " << distances[0][1] << endl;
    cerr << "distance 1 and 2: " << distances[1][2] << endl;
}

void TravelThief::sample_solver()
{
    // sample distances;
    distances.resize(4);
    for(auto & distance : distances)
    {
        distance.resize(4, 0.0);
    }
    distances[0][1] = distances[1][0] = 5;
    distances[0][2] = distances[2][0] = 6;
    distances[0][3] = distances[3][0] = 6;
    distances[1][2] = distances[2][1] = 5;
    distances[1][3] = distances[3][1] = 6;
    distances[2][3] = distances[3][2] = 4;

    // sample items;
    items.resize(6);
    items[0] = {20, 2, 1};
    items[1] = {30, 3, 1};
    items[2] = {100, 3,2};
    items[3] = {40, 1,2};
    items[4] = {40, 1,2};
    items[5] = {20, 2,3};

    // capacity
    capacity_of_knapsack = 3;
}

void TravelThief::print_items()
{
    cerr << "num items: " << items.size() << endl;
    for(int i=0;i<items.size();i++)
    {
        cerr << i << " " << items[i].profit << " " << items[i].weight << " " << items[i].assigned_node << endl;
    }
}
