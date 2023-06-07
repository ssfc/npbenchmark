//
// Created by take_ on 2023/6/1.
//

#ifndef LKH_TRAVEL_THIEF_H
#define LKH_TRAVEL_THIEF_H

#include <string>
#include <utility>
#include <vector>

struct City{
    int x;
    int y;
    std::vector<int> contained_items;  // list of items in this city;
    double distance_to_dest;
};

struct Item{
    int value;
    int weight;
    int assigned_city;
    double item_travel_time;
    double thief_travel_time_withonlythis; // 从出发算起，小偷空载到达xi, 再背着Ixik到达终点（也就是起点）的总时间
    double score;
};

class TravelThief{
    // parameter directly from file;
    std::string problem_name;
    std::string knapsack_data_type;
    int num_cities;
    int num_items;
    int capacity;
    double min_speed;
    double max_speed;
    double renting_ratio;
    std::string edge_weight_type;
    std::vector<City> cities;
    std::vector<std::vector<double>> city2city_distances;
    std::vector<Item> items;

    // computed from the above;
    double speed_capacity_ratio;
    double object_value;

    // concerning LKH;
    double tour_length_LKH; // tour length provided by LKH;
    std::vector<int> tour; // tsp path generated;
    double tour_length_computed; // tour length added by myself;
    double total_traveling_time; // total travel time with no item, 不难看出在路径构筑后这是一个恒定值;

public:
    TravelThief();
    ~TravelThief();

    void print_city_coords();

    void print_items();

    void print_tour();

    void compute_city_distances();

    double compute_total_distances();

    // void sample_solver();

    double simple_heuristic(); // Algorithm 1
};

#endif //LKH_TRAVEL_THIEF_H
