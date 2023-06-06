//
// Created by take_ on 2023/6/1.
//

#ifndef LKH_TRAVEL_THIEF_H
#define LKH_TRAVEL_THIEF_H

#include <string>
#include <utility>
#include <vector>

struct CityCoord{
    int x;
    int y;
};

struct Item{
    int profit;
    int weight;
    int assigned_city;
    double travel_time;
};

class TravelThief{
    std::string problem_name;
    std::string knapsack_data_type;
    int num_cities;
    int num_items;
    int capacity;
    double min_speed;
    double max_speed;
    double renting_ratio;
    std::string edge_weight_type;
    std::vector<CityCoord> city_coords;
    std::vector<std::vector<double>> city2city_distances;
    std::vector<Item> items;
    std::vector<std::vector<int>> city_contained_items;  // city_contained_items[i] are list of items in city i
    double object_value;

    double tour_length_LKH; // tour length provided by LKH;
    std::vector<int> tour; // tsp path generated;
    double tour_length_computed; // tour length added by myself;

public:
    TravelThief();
    ~TravelThief();

    void print_city_coords();

    void print_items();

    void print_tour();

    void compute_city_distances();

    double compute_total_distances();

    void sample_solver();

    double simple_heuristic(); // Algorithm 1
};

#endif //LKH_TRAVEL_THIEF_H
