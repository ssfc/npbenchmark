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
};

#endif //TPPCPP_TRAVEL_THIEF_H
