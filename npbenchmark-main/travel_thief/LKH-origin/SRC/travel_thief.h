//
// Created by take_ on 2023/6/1.
//

#ifndef LKH_TRAVEL_THIEF_H
#define LKH_TRAVEL_THIEF_H

#include <string>
#include <utility>
#include <vector>

struct NodeCoord{
    int x;
    int y;
};

struct Item{
    int profit;
    int weight;
    int assigned_node;
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
    std::vector<NodeCoord> city_coords;
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

    void set_knapsack_data_type(std::string input_knapsack_data_type) {
        knapsack_data_type = std::move(input_knapsack_data_type);
    }
    std::string get_knapsack_data_type() {
        return knapsack_data_type;
    }

    void set_dimension(int input_dimension) {
        num_cities = input_dimension;
    }
    [[nodiscard]] int get_dimension() const {
        return num_cities;
    }

    void set_num_items(int input_num_items) {
        num_items = input_num_items;
    }
    [[nodiscard]] int get_num_items() const {
        return num_items;
    }

    void set_capacity_of_knapsack(int input_num_items) {
        capacity = input_num_items;
    }
    [[nodiscard]] int get_capacity_of_knapsack() const {
        return capacity;
    }

    void set_min_speed(double input_min_speed) {
        min_speed = input_min_speed;
    }
    [[nodiscard]] double get_min_speed() const {
        return min_speed;
    }

    void set_max_speed(double input_max_speed) {
        max_speed = input_max_speed;
    }
    [[nodiscard]] double get_max_speed() const {
        return max_speed;
    }

    void set_renting_ratio(double input_max_speed) {
        renting_ratio = input_max_speed;
    }
    [[nodiscard]] double get_renting_ratio() const {
        return renting_ratio;
    }

    void set_edge_weight_type(std::string input_edge_weight_type) {
        edge_weight_type = std::move(input_edge_weight_type);
    }
    [[nodiscard]] std::string get_edge_weight_type() const {
        return edge_weight_type;
    }

    std::vector<NodeCoord> get_city_coords()
    {
        return city_coords;
    }

    void set_length(double input_length)
    {
        tour_length_LKH = input_length;
    }

    double get_length() const
    {
        return tour_length_LKH;
    }

    void add_node_coords(NodeCoord this_node_coord)
    {
        city_coords.push_back(this_node_coord);
    }

    void print_node_coords();

    void add_item(Item this_item)
    {
        items.push_back(this_item);
    }

    void print_items();

    void add_tour(int city_id)
    {
        tour.push_back(city_id);
    }

    void print_tour();

    void compute_city_distances();

    double compute_total_distances();

    void sample_solver();

    double simple_heuristic(); // Algorithm 1
};

#endif //LKH_TRAVEL_THIEF_H
