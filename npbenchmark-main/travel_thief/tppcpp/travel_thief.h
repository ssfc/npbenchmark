//
// Created by take_ on 2023/5/9.
//

#ifndef TPPCPP_TRAVEL_THIEF_H
#define TPPCPP_TRAVEL_THIEF_H

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
    int dimension;
    int num_items;
    int capacity_of_knapsack;
    double min_speed;
    double max_speed;
    double renting_ratio;
    std::string edge_weight_type;
    std::vector<NodeCoord> node_coords;
    std::vector<std::vector<double>> distances;
    std::vector<Item> items;
    std::vector<std::vector<int>> city_contained_items; // city_contained_items[i] are list of items in city i

public:
    TravelThief();
    ~TravelThief();

    void set_problem_name(std::string input_problem_name) {
        problem_name = std::move(input_problem_name);
    }
    std::string get_problem_name() {
        return problem_name;
    }

    void set_knapsack_data_type(std::string input_knapsack_data_type) {
        knapsack_data_type = std::move(input_knapsack_data_type);
    }
    std::string get_knapsack_data_type() {
        return knapsack_data_type;
    }

    void set_dimension(int input_dimension) {
        dimension = input_dimension;
    }
    [[nodiscard]] int get_dimension() const {
        return dimension;
    }

    void set_num_items(int input_num_items) {
        num_items = input_num_items;
    }
    [[nodiscard]] int get_num_items() const {
        return num_items;
    }

    void set_capacity_of_knapsack(int input_num_items) {
        capacity_of_knapsack = input_num_items;
    }
    [[nodiscard]] int get_capacity_of_knapsack() const {
        return capacity_of_knapsack;
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

    void add_node_coords(NodeCoord this_node_coord)
    {
        node_coords.push_back(this_node_coord);
    }

    void print_node_coords();

    void add_item(Item this_item)
    {
        items.push_back(this_item);
    }

    void compute_distances();

    void solver();

    void print_items();
};

#endif //TPPCPP_TRAVEL_THIEF_H
