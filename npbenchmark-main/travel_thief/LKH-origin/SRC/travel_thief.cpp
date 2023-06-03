//
// Created by take_ on 2023/6/1.
//

#include "travel_thief.h"
#include <cmath>
#include <iostream>

using namespace std;

TravelThief::TravelThief():
        num_cities(0),
        num_items(0),
        capacity(0),
        min_speed(0.0),
        max_speed(0.0),
        renting_ratio(0.0),
        object_value(0.0),
        tour_length_LKH(0.0),
        tour_length_computed(0.0)
{

}

TravelThief::~TravelThief()
= default;

void TravelThief::print_node_coords()
{
    cerr << "num node coords: " << city_coords.size() << endl;
    for(int i=0; i < city_coords.size(); i++)
    {
        cerr << i << " " << city_coords[i].x << " " << city_coords[i].y << endl;
    }
}

void TravelThief::compute_city_distances()
{
    city2city_distances.resize(city_coords.size());
    for(int i=0; i < city_coords.size(); i++)
    {
        city2city_distances[i].resize(city_coords.size(), 0.0);
    }

    for(int i=0; i < city_coords.size(); i++)
    {
        for(int j=0; j < city_coords.size(); j++)
        {
            city2city_distances[i][j] = city2city_distances[j][i] = sqrt(
                    (city_coords[i].x - city_coords[j].x) * (city_coords[i].x - city_coords[j].x)
                    + (city_coords[i].y - city_coords[j].y) * (city_coords[i].y - city_coords[j].y));
        }
    }

    cerr << "distance 0 and 1: " << city2city_distances[0][1] << endl;
    cerr << "distance 1 and 2: " << city2city_distances[1][2] << endl;
}

double TravelThief::compute_total_distances()
{
    tour_length_computed = 0.0;
    for(int i=0;i<tour.size()-1;i++)
    {
        // cerr << "This distance: " << city2city_distances[tour[i]][tour[i+1]] << endl;
        // int truncated = static_cast<int>(city2city_distances[tour[i]][tour[i+1]] * 2);
        // double result = static_cast<double>(truncated) / 2;
        // total_distance += result;
        tour_length_computed += city2city_distances[tour[i]][tour[i+1]];
    }

    // cerr << "This distance: " << city2city_distances[tour[tour.size()-1]][tour[0]] << endl;
    // int truncated = static_cast<int>(city2city_distances[tour[tour.size()-1]][tour[0]] * 2);
    // double result = static_cast<double>(truncated) / 2;
    // total_distance += result;
    tour_length_computed += city2city_distances[tour[tour.size()-1]][tour[0]];

    return tour_length_computed;
}

void TravelThief::sample_solver()
{
    problem_name = "sample name";
    knapsack_data_type = "sample knapsack type";
    num_cities = 4;
    num_items = 6;

    capacity = 3; // capacity
    min_speed = 0.1;
    max_speed = 1;
    renting_ratio = 1;
    edge_weight_type = "sample edge type";

    // sample distances;
    city2city_distances.resize(num_cities);
    for(auto & distance : city2city_distances)
    {
        distance.resize(num_cities, 0.0);
    }
    city2city_distances[0][1] = city2city_distances[1][0] = 5;
    city2city_distances[0][2] = city2city_distances[2][0] = 6;
    city2city_distances[0][3] = city2city_distances[3][0] = 6;
    city2city_distances[1][2] = city2city_distances[2][1] = 5;
    city2city_distances[1][3] = city2city_distances[3][1] = 6;
    city2city_distances[2][3] = city2city_distances[3][2] = 4;

    // sample items;
    items.resize(num_items);
    items[0] = {20, 2, 1};
    items[1] = {30, 3, 1};
    items[2] = {100, 3,2};
    items[3] = {40, 1,2};
    items[4] = {40, 1,2};
    items[5] = {20, 2,3};

    double speed_capacity_ratio = (max_speed - min_speed) / capacity;
    cerr << "speed capacity ratio: " << speed_capacity_ratio << endl;

    // object value of tour (0, 1, 3, 2) and object (0, 0, 0, 1, 1, 0)
    object_value = 40 + 40 - 1 * (6/(1-0.3*2) + 5/1.0 + 6/1.0 + 4/1.0);
    cerr << "object value: " << object_value << endl;
}

void TravelThief::print_items()
{
    cerr << "num items: " << items.size() << endl;
    for(int i=0;i<items.size();i++)
    {
        cerr << i << " " << items[i].profit << " " << items[i].weight << " " << items[i].assigned_node << endl;
    }
}

void TravelThief::print_tour()
{
    cerr << "Tour (size " << tour.size() << "): ";
    for (int cityId : tour)
    {
        cerr << cityId << " ";
    }
    cerr << endl;
}