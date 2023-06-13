//
// Created by take_ on 2023/6/13.
//
//
// Created by take_ on 2023/6/1.
//
extern "C" int compute_tsp(); // 声明c文件中的函数原型

#include "travel_thief.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <set>
#include <sstream>
#include <string>

using namespace std;

TravelThief::TravelThief(const string& input_file_with_path, int max_runtime):
        num_cities(0),
        num_items(0),
        capacity(0),
        min_speed(0.0),
        max_speed(0.0),
        renting_ratio(0.0),
        used_capacity(0),
        object_value(0.0),
        tour_length_LKH(0.0),
        tour_length_computed(0.0),
        total_traveling_time(0.0)
{
    start_time = clock();
    int input_seed = 1;
    init_rand(input_seed); // initialize random generator;

    size_t lastSlashPos = input_file_with_path.find_last_of('/'); // 查找最后一个斜杠的位置
    filename = input_file_with_path.substr(lastSlashPos + 1); // 截取字符串

    //string filename = "../data/a280_n279_bounded-strongly-corr_01.ttp";
    // string filename = "../data/a280_n1395_uncorr-similar-weights_05.ttp";
    // string filename = "../data/fnl4461_n4460_bounded-strongly-corr_01.ttp";
    // string filename = "../data/pla33810_n33809_bounded-strongly-corr_01.ttp";
    // string filename = "../data/fnl4461_n13380_uncorr_02.ttp";
    ifstream file(input_file_with_path);

    if(!file.is_open())
    {
        cerr << "Failed to open file " << input_file_with_path << endl;
    }

    int num_city_coord = 0;
    string line;
    while (getline(file, line))
    {
        if (line.find("PROBLEM NAME:") != string::npos)
        {
            line.erase(0, 14); // 删除"PROBLEM NAME: "，保留问题名称
            problem_name = line;
            cerr << "PROBLEM NAME:" << problem_name << endl;
        }
        else if (line.find("KNAPSACK DATA TYPE:") != string::npos)
        {
            line.erase(0, 20); // 删除"KNAPSACK DATA TYPE: "，保留背包数据类型
            knapsack_data_type = line;
            cerr << "KNAPSACK DATA TYPE:" << knapsack_data_type << endl;
        }
        else if (line.find("DIMENSION:") != string::npos)
        {
            line.erase(0, 11); // 删除"DIMENSION: "，保留维度
            num_cities = stoi(line);
            cerr << "DIMENSION:" << num_cities << endl;
        }
        else if (line.find("NUMBER OF ITEMS:") != string::npos)
        {
            line.erase(0, 17); // 删除"NUMBER OF ITEMS: "，保留物品数量
            num_items = stoi(line);
            cerr << "NUMBER OF ITEMS:" << num_items << endl;
        }
        else if (line.find("CAPACITY OF KNAPSACK:") != string::npos)
        {
            line.erase(0, 22); // 删除"CAPACITY OF KNAPSACK: "，保留背包容量
            capacity = stoi(line);
            cerr << "CAPACITY OF KNAPSACK:" << capacity << endl;
        }
        else if (line.find("MIN SPEED:") != string::npos)
        {
            // cerr << "MIN SPEED find" << endl;
            line.erase(0, 11); // 删除"MIN SPEED: "，保留最小速度
            min_speed = stod(line);
            cerr << "MIN SPEED:" << min_speed << endl;
        }
        else if (line.find("MAX SPEED:") != string::npos)
        {
            // cerr << "MAX SPEED find" << endl;
            line.erase(0, 11); // 删除"MAX SPEED: "，保留最大速度
            max_speed = stod(line);
            cerr << "MAX SPEED:" << max_speed << endl;
        }
        else if (line.find("RENTING RATIO:") != string::npos)
        {
            line.erase(0, 14); // 删除"RENTING RATIO: "，保留租金比率
            renting_ratio = stod(line);
            cerr << "RENTING RATIO:" << renting_ratio << endl;
        }
        else if (line.find("EDGE_WEIGHT_TYPE:") != string::npos)
        {
            line.erase(0, 18); // 删除"EDGE_WEIGHT_TYPE: "，保留边权类型
            edge_weight_type = line;
            cerr << "EDGE_WEIGHT_TYPE:" << edge_weight_type << endl;
        }
        else if (line.find("NODE_COORD_SECTION") != string::npos)
        {
            cerr << "NODE_COORD_SECTION (INDEX, X, Y): " << endl;
            while (getline(file, line))
            {
                if (line.find("ITEMS SECTION") == string::npos)
                {
                    int index, x, y;
                    // 从字符串中解析出节点索引、横坐标、纵坐标
                    replace(line.begin(), line.end(), '\t', ' ');
                    istringstream iss(line);
                    iss >> index >> x >> y;
                    City temp{};
                    temp.index = index - 1;
                    temp.x = x;
                    temp.y = y;
                    cities.push_back(temp);

                    // cerr << index << " " << temp.x << " " << temp.y << endl; // 输出节点坐标
                    num_city_coord++;
                }
                else
                {
                    cerr << "ITEMS SECTION (INDEX, PROFIT, WEIGHT, ASSIGNED NODE NUMBER): " << endl;
                    while (getline(file, line))
                    {
                        if (line.empty())
                        {
                            break; // 节点坐标结束
                        }
                        int index, value, weight, assigned_city;
                        // 从字符串中解析出节点索引、横坐标、纵坐标
                        replace(line.begin(), line.end(), '\t', ' ');
                        istringstream iss(line);
                        iss >> index >> value >> weight >> assigned_city;
                        // 输出节点坐标
                        // cerr << index << " " << profit << " " << weight << " " << assigned_city << endl;
                        Item temp{};
                        temp.index = index-1;
                        temp.value = value;
                        temp.weight = weight;
                        temp.assigned_city = assigned_city - 1;
                        temp.packing_status = false;
                        items.push_back(temp);
                        cities[temp.assigned_city].contained_items.push_back(temp);
                    }
                }
            }
        }
    }

    file.close();
    // cerr << "num_city_coord: " << num_city_coord << endl;
    // cerr << "test_num_item_section: " << test_num_item_section << endl;
    // test.print_city_coords();
    // test.print_items();

    speed_capacity_ratio = (max_speed - min_speed) / capacity;
    cerr << "speed capacity ratio: " << speed_capacity_ratio << endl;
}

TravelThief::~TravelThief()
= default;

void TravelThief::print_city_coords()
{
    cerr << "num city coords: " << cities.size() << endl;
    for(int i=0; i < cities.size(); i++)
    {
        cerr << i << " " << cities[i].x << " " << cities[i].y << endl;
    }
}

void TravelThief::compute_city_distances()
{
    city2city_distances.resize(cities.size());
    for(int i=0; i < cities.size(); i++)
    {
        city2city_distances[i].resize(cities.size(), 0.0);
    }

    for(int i=0; i < cities.size(); i++)
    {
        for(int j=0; j < cities.size(); j++)
        {
            city2city_distances[i][j] = city2city_distances[j][i] = sqrt(
                    (cities[i].x - cities[j].x) * (cities[i].x - cities[j].x)
                    + (cities[i].y - cities[j].y) * (cities[i].y - cities[j].y));
        }
    }

    cerr << "distance 0 and 1: " << city2city_distances[0][1] << endl;
    cerr << "distance 1 and 2: " << city2city_distances[1][2] << endl;
}

double TravelThief::compute_total_distances()
{
    tour_length_computed = 0.0;

    tour_length_computed += city2city_distances[tour[0]][tour[tour.size()-1]];
    cities[tour[tour.size()-1]].distance_to_dest = tour_length_computed;

    for(int i=int(tour.size())-1;i>0;i--)
    {
        // cerr << "This distance: " << city2city_distances[tour[i]][tour[i+1]] << endl;
        // int truncated = static_cast<int>(city2city_distances[tour[i]][tour[i+1]] * 2);
        // double result = static_cast<double>(truncated) / 2;
        // total_distance += result;
        tour_length_computed += city2city_distances[tour[i]][tour[i-1]];
        cities[tour[i-1]].distance_to_dest = tour_length_computed;
    }

    // cerr << "This distance: " << city2city_distances[tour[tour.size()-1]][tour[0]] << endl;
    // int truncated = static_cast<int>(city2city_distances[tour[tour.size()-1]][tour[0]] * 2);
    // double result = static_cast<double>(truncated) / 2;
    // total_distance += result;

    return tour_length_computed;
}

/*
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

    // object value of tour (0, 1, 3, 2) and object (0, 0, 0, 1, 1, 0)
    object_value = 40 + 40 - 1 * (6/(1-0.3*2) + 5/1.0 + 6/1.0 + 4/1.0);
    cerr << "object value: " << object_value << endl;
}
 */

void TravelThief::print_items()
{
    cerr << "num items: " << items.size() << endl;
    for(int i=0;i<items.size();i++)
    {
        cerr << i << " " << items[i].value << " " << items[i].weight << " " << items[i].assigned_city << endl;
    }
}

void TravelThief::print_tour()
{
    cerr << "Tour (size " << tour.size() << "): ";
    for (int city_index : tour)
    {
        cerr << city_index << " (";
        for(Item& this_item : cities[city_index].contained_items)
        {
            cerr << this_item.index << " ";
        }
        cerr << ") ";
    }
    cerr << endl;
}

double TravelThief::compute_object_value(vector<City>& input_cities)
{
    double weight_leaving = 0.0;
    double collect_time = 0.0;
    double total_value = 0.0; // 偷盗物品的总价值
    // cerr << "tour with picked items: ";
    for(int i=0; i<tour.size(); i++)
    {
        if(!input_cities[tour[i]].picked_items.empty())
        {
            // cerr << "city " << tour[i] << " (";
            for(Item& this_item : input_cities[tour[i]].picked_items)
            {
                // cerr << this_item.index << " ";
                weight_leaving += this_item.weight;
                total_value += this_item.value;
            }
            // cerr << ") ";
        }
        // cerr << "weight " << weight_leaving << " ";

        if(i != tour.size()-1)
        {
            collect_time += city2city_distances[tour[i]][tour[i+1]] / (max_speed - speed_capacity_ratio * weight_leaving);
        }

        // cerr << "collect time " << collect_time << " ";
    }
    // cerr << endl;

    // 从最后一个城市返回出发点的时间
    // cerr << "used capacity: " << used_capacity << endl;

    // Implement A1 LINE 16
    // Set the resulting objective value
    // Z∗:= max (Z(Π, P), −R × t′)
    // Z*: object value
    // Z(Π, P): 扣除租金物品的价值
    double back_time = city2city_distances[tour[tour.size()-1]][tour[0]]
                       / (max_speed - speed_capacity_ratio * used_capacity);
    // cerr << "distance: " << city2city_distances[tour[tour.size()-1]][tour[0]] << endl;
    // cerr << "used capacity: " << used_capacity << endl;
    // cerr << "back time: " << back_time << endl;
    // evaluate total value
    // cerr << "total value: " << total_value << endl;
    // cerr << "the latter part: " << - renting_ratio * (back_time + collect_time) << endl;
    // evaluate back time
    // cerr << "back time: " << back_time << endl;
    // cerr << "back time base: " << city2city_distances[tour[tour.size()-1]][tour[0]] << endl;
    // evaluate collect time
    // cerr << "collect time: " << collect_time << endl;
    /*
    double collect_time_base = 0;
    for(int i = 0; i<tour.size()-1; i++)
    {
        collect_time_base += city2city_distances[tour[i]][tour[i+1]];
    }
    cerr << "collect time base: " << collect_time_base << endl;
     */

    // cerr << "total time: " << back_time + collect_time << endl;
    double output_object_value = total_value - renting_ratio * (back_time + collect_time);
    // cerr << "object value: " << output_object_value << endl;

    return output_object_value;
}

bool TravelThief::checkTour() {
    std::set<int> expectedValues;

    // 添加期望的整数值到 expectedValues
    for (int i = 0; i < tour.size(); i++) {
        expectedValues.insert(i);
    }

    // 检查 tour 中的元素是否在 expectedValues 中
    for (int value : tour) {
        auto it = expectedValues.find(value);
        if (it != expectedValues.end()) {
            expectedValues.erase(it);
        }
    }

    // 检查 expectedValues 是否为空
    return expectedValues.empty();
}

void TravelThief::compute_object_value_by_saved_result(string result_filename)
{
    compute_city_distances();

    // 打开文件
    ifstream file(result_filename);
    if (file.is_open())
    {
        string line;
        vector<int> packing_plan;

        // 逐行读取文件内容
        while (getline(file, line))
        {
            // 移除空格和括号
            line.erase(remove(line.begin(), line.end(), ' '), line.end());
            line.erase(remove(line.begin(), line.end(), '['), line.end());
            line.erase(remove(line.begin(), line.end(), ']'), line.end());

            // 解析数字
            vector<int> numbers;
            string number;
            for (char c : line)
            {
                if (c == ',')
                {
                    numbers.push_back(stoi(number) - 1);
                    number.clear();
                }
                else
                {
                    number += c;
                }
            }
            numbers.push_back(stoi(number) - 1);

            // 保存到对应的数组中
            if (tour.empty())
            {
                tour = numbers;
            }
            else
            {
                packing_plan = numbers;
            }
        }

        // 打印结果
        cerr << "Tour: ";
        for (int num : tour)
        {
            cerr << num << " ";
        }
        cerr << endl;

        cerr << "Packing Plan: ";
        for (int num : packing_plan)
        {
            cerr << num << " ";
        }
        cerr << endl;

        sort(items.begin(), items.end(), [](const Item& item1, const Item& item2) {
            return item1.index < item2.index;
        });

        for(int this_item_index : packing_plan)
        {
            // Add this item to city picked items;
            cities[items[this_item_index].assigned_city].picked_items.push_back(items[this_item_index]);
            used_capacity += items[this_item_index].weight;
        }

        // 关闭文件
        file.close();
    }
    else
    {
        cerr << "Failed to open the file." << endl;
    }

    if(used_capacity <= capacity)
    {
        cerr << "used capacity legal" << endl;
    }
    else
    {
        cerr << "used capacity overweight!" << endl;
    }

    if (checkTour()) {
        cerr << "tour contains all cities." << std::endl;
    } else {
        cerr << "tour is missing some cities." << std::endl;
    }

    object_value = compute_object_value(cities);
    cerr << "object_value: " << object_value << endl;
}

