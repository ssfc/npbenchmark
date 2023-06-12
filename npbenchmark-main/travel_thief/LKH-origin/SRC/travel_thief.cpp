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

void TravelThief::save_result()
{
    cerr << "filename: " << filename << endl;
    string save_filename = filename + ".thisIsMyAlgorithm." + to_string(clock());
    ofstream result_file(save_filename);
    if (result_file.is_open())
    {
        // save tour cities;
        result_file << "[";

        for (int i = 0; i < tour.size(); i++)
        {
            result_file << tour[i] + 1; // number starts with 1
            if (i != tour.size() - 1)
            {
                result_file << ",";
            }
        }

        result_file << "]" << endl;

        sort(items.begin(), items.end(), [](const Item& item1, const Item& item2) {
            return item1.index < item2.index;
        });
        vector<int> packing_plan;
        for(Item& this_item : items)
        {
            if(this_item.packing_status)
            {
                packing_plan.push_back(this_item.index);
            }
        }

        cerr << "packing plan size: " << packing_plan.size() << endl;

        // save packed items;
        result_file << "[";

        for (int i = 0; i < packing_plan.size(); i++)
        {
            result_file << packing_plan[i] + 1; // number starts with 1
            if (i != packing_plan.size() - 1)
            {
                result_file << ",";
            }
        }

        result_file << "]" << endl;

        result_file.close();
        cerr << "Save file created successfully.\n";
    }
    else
    {
        cerr << "Unable to create parameter file.\n";
    }
}

void TravelThief::compute_object_value_by_saved_result()
{
    object_value = compute_object_value(cities);
    cerr << "object_value: " << object_value << endl;
}

// Algorithm 1
double TravelThief::simple_heuristic()
{
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
            tsp_file << i+1 << " " << cities[i].x * scale << " " << cities[i].y * scale << "\n";
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

    string line;
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

            int city_index = stoi(line);
            // LKH官方程序是从1开始算城市的，所以要-1;
            // Implement A1 LINE 1
            // Fill the array D with values dxi, xi ∈ {x2, . . . , xn}
            // dxi: total travel distance of item Ixik // 这个显然要从被捡起来开始算。
            // xi: city index
            tour.push_back(city_index-1);
        }
    }

    // Evaluate A1 LINE 1
    print_tour();

    // output tour length
    cerr << "Length: " << tour_length_LKH / scale << endl;
    cerr << "Length computed: " << compute_total_distances() << endl;

    // Implement A1 LINE 2
    // Calculate the total traveling time t′
    // t': total traveling time without item
    total_traveling_time = tour_length_LKH / scale; // 因为最大速度是1, 所以不除了;
    // Evaluate A1 LINE 2
    cerr << "total traveling time: " << total_traveling_time << endl;

    // Implement A1 LINE 3
    // for all items Ixik, xi ∈ Π, k ∈ Mxi do
    // Ixik:  item k in city xi
    // xi: city index
    // Π: tour path
    // Mxi: items in city xi
    cerr << "tour city: " << endl;
    for(int city_index: tour)
    {
        // Evaluate A1 LINE 3
        // evaluate travel distance of each city on the tour;
        // cerr << city_index << " (" << cities[city_index].travel_distance << ") ";
        // show each city contained items
        cerr << "city " << city_index;
        for(Item& this_item_in_city : cities[city_index].contained_items)
        {
            // Implement A1 LINE 4
            // Calculate txik by using Equation 1
            // txik: total travel time of item Ixik // 这个显然要从被捡起来开始算。
            items[this_item_in_city.index].item_travel_time = cities[city_index].distance_to_dest
                    / (max_speed - speed_capacity_ratio * items[this_item_in_city.index].weight);

            // Implement A1 LINE 5
            // Set t′xik := t′ − dxi + txik
            // t': total traveling time without item
            // dxi: total travel distance of item Ixik // 这个显然要从被捡起来开始算。
            // txik: total travel time of item Ixik // 这个显然要从被捡起来开始算。
            // meaning: 从出发算起，小偷空载到达xi, 再背着Ixik到达终点（也就是起点）的总时间
            items[this_item_in_city.index].thief_travel_time_withonlythis = total_traveling_time
                    - cities[city_index].distance_to_dest + items[this_item_in_city.index].item_travel_time;

            // Implement A1 LINE 6;
            // A1 LINE 6
            // Set scorexik := pxik − R × txik
            // scorexik: 给每个物品打分
            // xi: city index
            // pxik: 物品原本价值
            // R: 单位时间租金
            // txik: travel time of item Ixik from being picked up to being transported to the destination
            // meaning: 物品经过时间贬值后剩下多少
            items[this_item_in_city.index].score = items[this_item_in_city.index].value -
                    renting_ratio * items[this_item_in_city.index].item_travel_time;

            // A1 LINE 7
            // Set uxik := R × t′ + (pxik − R × t′xik)
            // R: 单位时间租金
            // t': total traveling time without item
            // pxik: value of item Ixik
            // t'xik: 从出发算起，小偷空载到达xi, 再背着Ixik到达终点（也就是起点）的总时间
            // meaning: fitness value
            items[this_item_in_city.index].fitness = renting_ratio * total_traveling_time
                    + (items[this_item_in_city.index].value
                    - renting_ratio * items[this_item_in_city.index].thief_travel_time_withonlythis);

            // Evaluate A1 LINE 4 LINE 5 LINE 6 LINE 7;
            cerr << "\titem " << this_item_in_city.index
                 << "\tdistance to dest " << cities[city_index].distance_to_dest
                 << "\ttime " << items[this_item_in_city.index].item_travel_time
                 << "\tthief time " << items[this_item_in_city.index].thief_travel_time_withonlythis
                 << "\tscore " << items[this_item_in_city.index].score
                 << "\tfitness " << items[this_item_in_city.index].fitness << endl;
        }
        // cerr << endl;

    }

    // Implement A1 LINE 8;
    // Create the joint set of items I and sort them in descending order score values
    sort(items.begin(), items.end(), [](const Item& item1, const Item& item2) {
        return item1.score > item2.score;
    });

    // Evaluate A1 LINE 8;
    /*
    cerr << "Sort by score descending: ";
    for(Item& this_item : items)
    {
        // cerr << this_item.index << " (" << this_item.score << ") ";
    }
    cerr << endl;
     */

    // Implement A1 LINE 9 (and Evaluate A1 LINE 9)
    // Set the used capacity variable Wc := 0
    used_capacity = 0;
    object_value = 0.0;

    // Implement A1 LINE 10
    // for all items Ixik ∈ I do
    // Ixik: item k in city xi
    for(Item& this_item : items)
    {
        // Evaluate A1 LINE 10
        // cerr << this_item.score << " ";

        // A1 LINE 11
        // if (Wc + wxik < W) and (uxik > 0) then
        // Wc: used capacity
        // wxik: weight of item Ixik
        // W: max capacity allowed
        // uxik: fitness value
        if(used_capacity + this_item.weight < capacity && this_item.fitness > 0)
        {
            // cerr << this_item.fitness << " ";

            // Implement A1 LINE 12
            // Add the item Ixik to the packing plan P
            // Ixik: item
            this_item.packing_status = true;

            // Add this item to city picked items;
            cities[this_item.assigned_city].picked_items.push_back(this_item);

            // Implement A1 LINE 13
            // Increase the used capacity variable Wc := Wc + wxik
            // Wc: used capacity
            // wxik: weight of item Ixik
            used_capacity += this_item.weight;
        }

        // Implement A1 LINE 14
        // if (Wc = W) then
        // Wc: used capacity
        // W: capacity
        // Evaluate A1 LINE 14
        // used_capacity = capacity;
        if(used_capacity == capacity)
        {
            cerr << "equal capacity" << endl;
            // Implement A1 LINE 15
            // Exit the loop
            break;
        }
    }

    object_value = compute_object_value(cities);
    cerr << "object value: " << object_value << endl;

    double elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    cerr << "Running time(s): " << elapsed_time << endl;

    return object_value;
}

void TravelThief::evaluate_add_one_item()
{
    // evaluate add one item;
    double total_value = items[240].value;
    cerr << "expected total value: " << total_value << endl;
    double back_time = city2city_distances[tour[tour.size()-1]][tour[0]]
                       / (max_speed - speed_capacity_ratio * items[240].weight);
    cerr << "expected back time: " << back_time << endl;
    double collect_time = city2city_distances[tour[0]][tour[1]] + city2city_distances[tour[1]][tour[2]]
    + cities[241].distance_to_dest / (max_speed - speed_capacity_ratio * items[240].weight) - back_time;
    cerr << "expected collect time: " << collect_time << endl;
    // cerr << "city 241 distance to dest: " << cities[241].distance_to_dest << endl;
    double expected_object_value = total_value - renting_ratio * (back_time + collect_time);
    cerr << "expected object value: " << expected_object_value << endl;
}

void TravelThief::evaluate_add_two_items()
{
    // evaluate add two items;
    double total_value = items[240].value + items[0].value;
    cerr << "expected total value: " << total_value << endl;
    double back_time = city2city_distances[tour[tour.size()-1]][tour[0]]
                       / (max_speed - speed_capacity_ratio * (items[240].weight + items[0].weight));
    cerr << "expected back time: " << back_time << endl;
    double collect_time = city2city_distances[tour[0]][tour[1]]
    + city2city_distances[tour[1]][tour[2]] / (max_speed - speed_capacity_ratio * items[0].weight)
    + cities[241].distance_to_dest / (max_speed - speed_capacity_ratio * (items[240].weight + items[0].weight))
    - back_time;
    cerr << "expected collect time: " << collect_time << endl;
    // cerr << "city 241 distance to dest: " << cities[241].distance_to_dest << endl;
    double expected_object_value = total_value - renting_ratio * (back_time + collect_time);
    cerr << "expected object value: " << expected_object_value << endl;
}

void TravelThief::evaluate_add_and_remove_item(int input_iter, unsigned int &input_rand_select)
{
    if(input_iter==0)
    {
        input_rand_select = 240;
        // evaluate add one item;
        // evaluate_add_one_item();
    }
    else if(input_iter==1)
    {
        input_rand_select = 0;

        // evaluate add two items;
        // evaluate_add_two_items();
    }
    else if(input_iter==2)
    {
        input_rand_select = 0;
    }
    else if(input_iter==3)
    {
        input_rand_select = 0;
    }
    else if(input_iter==4)
    {
        input_rand_select = 240;
    }
}

// Algorithm 2
double TravelThief::random_local_search()
{
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
            tsp_file << i+1 << " " << cities[i].x * scale << " " << cities[i].y * scale << "\n";
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

    string line;
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

            int city_index = stoi(line);
            // LKH官方程序是从1开始算城市的，所以要-1;
            // Implement A1 LINE 1
            // Fill the array D with values dxi, xi ∈ {x2, . . . , xn}
            // dxi: total travel distance of item Ixik // 这个显然要从被捡起来开始算。
            // xi: city index
            tour.push_back(city_index-1);
        }
    }

    // print_tour();

    // output tour length
    cerr << "Length: " << tour_length_LKH / scale << endl;
    cerr << "Length computed: " << compute_total_distances() << endl;

    // Implement A2 LINE 1
    // 1: Initialize P∗ such that no items are packed.
    // p*: previous packing status
    vector<Item> prev_items = items;
    vector<City> prev_cities = cities;
    int prev_used_capacity = used_capacity;
    double prev_object_value = object_value = compute_object_value(cities);

    // Evaluate A2 LINE 1
    /*
    cerr << "packing status (" << prev_items.size() << "): ";
    for(Item& this_item : prev_items)
    {
        cerr << this_item.packing_status << " ";
    }
    cerr << endl;
     */

    // Implement A2 LINE 2
    // repeat until no improvement for X iterations
    // X: RLS max iters
    int RLS_max_iters = 300000;
    int iter = 0;
    int output_interval = 500;
    // cerr << "object value of empty" << ": " << compute_object_value(cities) << endl;
    while(iter < RLS_max_iters)
    {
        // Evaluate A2 LINE 2
        if(iter % output_interval == 0)
        {
            cerr << "iter: " << iter << endl;
        }

        // Implement A2 LINE 3
        // Create P by inverting the packing status of a random picked item of P
        // P是改变一个元素的P*
        unsigned int rand_select = generated_random() % items.size();

        // evaluate adding and removing items
        // evaluate_add_and_remove_item(iter, rand_select);

        items[rand_select].packing_status = !items[rand_select].packing_status; // 取反

        // Evaluate A2 LINE 3
        /*
        cerr << rand_select << " "
        << items[rand_select].packing_status << endl;
         */

        int city_contained_rand_item = items[rand_select].assigned_city;
        if(items[rand_select].packing_status) // if true, means add new item
        {
            cities[city_contained_rand_item].picked_items.push_back(items[rand_select]);
            used_capacity += items[rand_select].weight;
            if(iter % output_interval == 0)
            {
                cerr << "object value of adding " << rand_select << ": "
                << compute_object_value(cities) << endl;
            }
        }
        else // if false, means remove item
        {
            auto it = std::find_if
                    (cities[city_contained_rand_item].picked_items.begin(),
                     cities[city_contained_rand_item].picked_items.end(),
                     [rand_select](const Item& item)
            {
                return item.index == rand_select;
            });

            if (it != cities[city_contained_rand_item].picked_items.end())
            {
                cities[city_contained_rand_item].picked_items.erase(it);
            }

            used_capacity -= items[rand_select].weight;

            if(iter % output_interval == 0)
            {
                cerr << "object value of deleting " << rand_select << ": "
                << compute_object_value(cities) << endl;
            }

        }

        object_value = compute_object_value(cities);

        // evaluate adding and removing item in packing plan
        /*
        int item_count = 0;
        for(const City& this_city : cities)
        {
            item_count += (int) (this_city.picked_items.size());
        }
        cerr << "total item count: " << item_count << endl;
         */

        // Implement A2 LINE 4
        // if Z(Π, P) ≥ Z(Π, P∗) and w(P) ≤ W then
        // Z(Π, P)改动后新的object_value
        if(object_value >= prev_object_value && used_capacity <= capacity)
        {
            prev_cities = cities;
            prev_items = items;
            prev_used_capacity = used_capacity;
            prev_object_value = object_value;
            if(iter % output_interval == 0)
            {
                cerr << "object value updated: " << object_value
                     << "\tused capacity: " << used_capacity << endl;
            }
        }
        else
        {
            cities = prev_cities;
            items = prev_items;
            used_capacity = prev_used_capacity;
            object_value = prev_object_value;
            if(iter % output_interval == 0)
            {
                cerr << "object value unchanged: " << object_value
                     << "\tused capacity: " << used_capacity << endl;
            }
        }

        iter++;
    }

    double elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    cerr << "Running time(s): " << elapsed_time << endl;

    return object_value;
}