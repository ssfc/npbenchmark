//
// Created by take_ on 2023/6/1.
//
extern "C" int compute_tsp(); // 声明c文件中的函数原型

#include "travel_thief.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>

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

void TravelThief::print_city_coords()
{
    cerr << "num city coords: " << city_coords.size() << endl;
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
        cerr << i << " " << items[i].profit << " " << items[i].weight << " " << items[i].assigned_city << endl;
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

// Algorithm 1
double TravelThief::simple_heuristic()
{
    string filename = "../data/a280_n279_bounded-strongly-corr_01.ttp";
    // string filename = "../data/a280_n1395_uncorr-similar-weights_05.ttp";
    // string filename = "../data/fnl4461_n4460_bounded-strongly-corr_01.ttp";
    // string filename = "../data/pla33810_n33809_bounded-strongly-corr_01.ttp";
    ifstream file(filename);

    if(!file.is_open())
    {
        cerr << "Failed to open file " << filename << endl;
        return 1;
    }

    int num_city_coord = 0;
    int test_num_item_section = 0;
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
            city_contained_items.resize(num_cities);
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
                    CityCoord temp = {x, y};
                    city_coords.push_back(temp);

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
                        int index, profit, weight, assigned_city;
                        // 从字符串中解析出节点索引、横坐标、纵坐标
                        replace(line.begin(), line.end(), '\t', ' ');
                        istringstream iss(line);
                        iss >> index >> profit >> weight >> assigned_city;
                        // 输出节点坐标
                        // cerr << index << " " << profit << " " << weight << " " << assigned_city << endl;
                        Item temp = {profit, weight, assigned_city - 1};
                        items.push_back(temp);
                        city_contained_items[assigned_city - 1].push_back(test_num_item_section);
                        test_num_item_section++;
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
    compute_city_distances();
    // test.sample_solver();

    // Set parameters;
    string parameter_filename = "pr2392.par";
    string problem_filename = "pr2392.tsp";
    int move_type = 5;
    int patching_c = 3;
    int patching_a = 2;
    int runs = 1;
    string output_filename = "output_tour.txt";

    // construct parameter file;
    ofstream parameter_file(parameter_filename);
    if (parameter_file.is_open())
    {
        parameter_file << "PROBLEM_FILE = " << problem_filename << "\n";
        // parameter_file << "OPTIMUM = 378032\n";
        parameter_file << "MOVE_TYPE = " << move_type << "\n";
        parameter_file << "PATCHING_C = " << patching_c << "\n";
        parameter_file << "PATCHING_A = " << patching_a << "\n";
        // parameter_file << "PRECISION = 90\n";
        parameter_file << "RUNS = " << runs << "\n";
        parameter_file << "OUTPUT_TOUR_FILE = " << output_filename << "\n";
        parameter_file.close();
        cerr << "Parameter file created successfully.\n";
    }
    else
    {
        cerr << "Unable to create parameter file.\n";
    }

    // Set problem
    string tsp_problem_name = "pr2392";
    string tsp_comment = "2392-city problem (Padberg/Rinaldi)";
    string tsp_type = "TSP";
    string tsp_edge_weight_type = "EUC_2D";
    int scale = 100; // 坐标放大倍数

    // construct TSP file;
    ofstream tsp_file(problem_filename);
    if (tsp_file.is_open())
    {
        tsp_file << "NAME : " << tsp_problem_name << "\n";
        tsp_file << "COMMENT : " << tsp_comment << "\n";
        tsp_file << "TYPE : " << tsp_type << "\n";
        tsp_file << "DIMENSION : " << num_cities << "\n";
        tsp_file << "EDGE_WEIGHT_TYPE : " << tsp_edge_weight_type << "\n";
        tsp_file << "NODE_COORD_SECTION\n";
        for(int i=0;i<num_cities;i++)
        {
            // LKH官方程序是从1开始算城市的，所以要+1;
            // tsp_file << i+1 << " " << city_coords[i].x << " " << city_coords[i].y << "\n";

            // 试着缩放一下坐标；
            tsp_file << i+1 << " " << city_coords[i].x * scale << " " << city_coords[i].y * scale << "\n";
        }
        tsp_file << "EOF\n";
        tsp_file.close();
        cerr << "TSP file created successfully.\n";
    }
    else
    {
        cerr << "Unable to create TSP file.\n";
    }

    // hello_world();
    compute_tsp();

    ifstream inputFile(output_filename);

    // string line;
    bool tourSectionReached = false;

    while (getline(inputFile, line))
    {
        if (line.find("COMMENT : Length =") != string::npos)
        {
            // 提取路径长度: Ah... 这个路径长度和自己手动累加的不一样呀;
            string lengthStr = line.substr(line.find('=') + 1);
            tour_length_LKH = stod(lengthStr);
        }

        if (line.find("TOUR_SECTION") != string::npos)
        {
            tourSectionReached = true;
            continue;
        }

        if (tourSectionReached)
        {
            if (line == "-1" || line == "EOF")
            {
                break;
            }

            int cityId = stoi(line);
            // LKH官方程序是从1开始算城市的，所以要-1;
            // A1 LINE 1
            tour.push_back(cityId-1);
        }
    }

    // output tour
    print_tour();

    // output tour length
    cerr << "Length: " << tour_length_LKH / scale << endl;
    cerr << "Length computed: " << compute_total_distances() << endl;

    // Implement A1 LINE 2
    double total_traveling_time = tour_length_LKH / scale; // 因为最大速度是1, 所以不除了;
    // Evaluate A1 LINE 2
    cerr << "total traveling time: " << total_traveling_time << endl;
    // Implement A1 LINE 3
    cerr << "tour city: " << endl;

    for(int city_id: tour)
    {
        cerr << city_id << ": ";
        for(int item_id : city_contained_items[city_id])
        {
            cerr << item_id << " ";
        }
        cerr << endl;
    }
    cerr << endl;

    return object_value;
}