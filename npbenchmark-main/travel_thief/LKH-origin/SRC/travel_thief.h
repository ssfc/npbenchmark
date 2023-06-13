//
// Created by take_ on 2023/6/1.
//

#ifndef LKH_TRAVEL_THIEF_H
#define LKH_TRAVEL_THIEF_H

#include <random>
#include <string>
#include <utility>
#include <vector>

struct Item{
    int index;
    int value;
    int weight;
    int assigned_city;
    double item_travel_time;
    double thief_travel_time_withonlythis; // 从出发算起，小偷空载到达xi, 再背着Ixik到达终点（也就是起点）的总时间
    double score;
    double fitness; // uxik
    bool packing_status;
};

struct City{
    int index;
    int x;
    int y;
    std::vector<Item> contained_items;  // list of items in this city;
    std::vector<Item> picked_items;
    double distance_to_dest;
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
    int used_capacity; // Wc
    double object_value;

    // concerning LKH;
    double tour_length_LKH; // tour length provided by LKH;
    std::vector<int> tour; // tsp path generated;
    double tour_length_computed; // tour length added by myself;
    double total_traveling_time; // total travel time with no item, 不难看出在路径构筑后这是一个恒定值;

    // debug variables:
    std::mt19937 generated_random;
    clock_t start_time;
    std::string filename;

public:
    TravelThief(const std::string& input_file_with_path, int max_runtime);
    ~TravelThief();

    void print_city_coords();

    void print_items();

    void print_tour();

    void compute_city_distances();

    double compute_total_distances();

    double compute_object_value(std::vector<City>& input_cities);

    // void sample_solver();

    double simple_heuristic(); // Algorithm 1
    double random_local_search(); // Algorithm 2

    // debug funcions
    void init_rand(int seed) { generated_random = std::mt19937(seed); }
    void evaluate_add_one_item();
    void evaluate_add_two_items();
    static void evaluate_add_and_remove_item(int iter, unsigned int &input_rand_select);
    void save_result();
    void compute_object_value_by_saved_result(std::string result_filename);
};

#endif //LKH_TRAVEL_THIEF_H
