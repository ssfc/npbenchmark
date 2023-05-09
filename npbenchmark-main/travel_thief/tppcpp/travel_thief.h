//
// Created by take_ on 2023/5/9.
//

#ifndef TPPCPP_TRAVEL_THIEF_H
#define TPPCPP_TRAVEL_THIEF_H

#include <string>
#include <utility>

class TravelThief{
    std::string problem_name;
    std::string knapsack_data_type;
    int dimension;
    int num_items;
    int capacity_of_knapsack;
    double min_speed;
    double max_speed;
    double renting_ratio;

public:
    // debug function
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
        max_speed = input_max_speed;
    }
    [[nodiscard]] double get_renting_ratio() const {
        return max_speed;
    }
};

#endif //TPPCPP_TRAVEL_THIEF_H
