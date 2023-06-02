//
// Created by take_ on 2023/6/1.
//
extern "C" int compute_tsp(); // 声明c文件中的函数原型

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

#include "travel_thief.h"

using namespace std;

int main()
{
    TravelThief test;
    string filename = "../data/a280_n279_bounded-strongly-corr_01.ttp";
    ifstream file(filename);

    if(!file.is_open())
    {
        cerr << "Failed to open file " << filename << endl;
        return 1;
    }

    int test_num_node_cord = 0;
    int test_num_item_section = 0;
    string line;
    while (getline(file, line))
    {
        if (line.find("PROBLEM NAME:") != string::npos)
        {
            line.erase(0, 14); // 删除"PROBLEM NAME: "，保留问题名称
            test.set_problem_name(line);
            cerr << "PROBLEM NAME:" << test.get_problem_name() << endl;
        }
        else if (line.find("KNAPSACK DATA TYPE:") != string::npos)
        {
            line.erase(0, 20); // 删除"KNAPSACK DATA TYPE: "，保留背包数据类型
            test.set_knapsack_data_type(line);
            cerr << "KNAPSACK DATA TYPE:" << test.get_knapsack_data_type() << endl;
        }
        else if (line.find("DIMENSION:") != string::npos)
        {
            line.erase(0, 11); // 删除"DIMENSION: "，保留维度
            test.set_dimension(stoi(line));
            cerr << "DIMENSION:" << test.get_dimension() << endl;
        }
        else if (line.find("NUMBER OF ITEMS:") != string::npos)
        {
            line.erase(0, 17); // 删除"NUMBER OF ITEMS: "，保留物品数量
            test.set_num_items(stoi(line));
            cerr << "NUMBER OF ITEMS:" << test.get_num_items() << endl;
        }
        else if (line.find("CAPACITY OF KNAPSACK:") != string::npos)
        {
            line.erase(0, 22); // 删除"CAPACITY OF KNAPSACK: "，保留背包容量
            test.set_capacity_of_knapsack(stoi(line));
            cerr << "CAPACITY OF KNAPSACK:" << test.get_capacity_of_knapsack() << endl;
        }
        else if (line.find("MIN SPEED:") != string::npos)
        {
            // cerr << "MIN SPEED find" << endl;
            line.erase(0, 11); // 删除"MIN SPEED: "，保留最小速度
            test.set_min_speed(stod(line));
            cerr << "MIN SPEED:" << test.get_min_speed() << endl;
        }
        else if (line.find("MAX SPEED:") != string::npos)
        {
            // cerr << "MAX SPEED find" << endl;
            line.erase(0, 11); // 删除"MAX SPEED: "，保留最大速度
            test.set_max_speed(stod(line));
            cerr << "MAX SPEED:" << test.get_max_speed() << endl;
        }
        else if (line.find("RENTING RATIO:") != string::npos)
        {
            line.erase(0, 14); // 删除"RENTING RATIO: "，保留租金比率
            test.set_renting_ratio(stod(line));
            cerr << "RENTING RATIO:" << test.get_renting_ratio() << endl;
        }
        else if (line.find("EDGE_WEIGHT_TYPE:") != string::npos)
        {
            line.erase(0, 18); // 删除"EDGE_WEIGHT_TYPE: "，保留边权类型
            test.set_edge_weight_type(line);
            cerr << "EDGE_WEIGHT_TYPE:" << test.get_edge_weight_type() << endl;
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
                    NodeCoord temp = {x, y};
                    test.add_node_coords(temp);

                    // cerr << index << " " << temp.x << " " << temp.y << endl; // 输出节点坐标
                    test_num_node_cord++;
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
                        int index, profit, weight, assigned_node;
                        // 从字符串中解析出节点索引、横坐标、纵坐标
                        replace(line.begin(), line.end(), '\t', ' ');
                        istringstream iss(line);
                        iss >> index >> profit >> weight >> assigned_node;
                        // 输出节点坐标
                        // cerr << index << " " << profit << " " << weight << " " << assigned_node << endl;
                        Item temp = {profit, weight, assigned_node - 1};
                        test.add_item(temp);
                        test_num_item_section++;
                    }
                }
            }
        }
    }

    file.close();
    // cerr << "test_num_node_cord: " << test_num_node_cord << endl;
    // cerr << "test_num_item_section: " << test_num_item_section << endl;
    // test.print_node_coords();
    // test.print_items();
    test.compute_distances();
    // test.sample_solver();

    // construct parameter file;
    ofstream parameter_file("pr2392.par");
    if (parameter_file.is_open())
    {
        parameter_file << "PROBLEM_FILE = pr2392.tsp\n";
        // parameter_file << "OPTIMUM = 378032\n";
        parameter_file << "MOVE_TYPE = 5\n";
        parameter_file << "PATCHING_C = 3\n";
        parameter_file << "PATCHING_A = 2\n";
        parameter_file << "RUNS = 10\n";
        parameter_file << "OUTPUT_TOUR_FILE = output_tour.txt\n";
        parameter_file.close();
        cerr << "Parameter file created successfully.\n";
    }
    else
    {
        cerr << "Unable to create parameter file.\n";
    }

    // construct TSP file;
    ofstream tsp_file("pr2392.tsp");
    if (tsp_file.is_open())
    {
        tsp_file << "NAME : pr2392\n";
        tsp_file << "COMMENT : 2392-city problem (Padberg/Rinaldi)\n";
        tsp_file << "TYPE : TSP\n";
        tsp_file << "DIMENSION : " << test.get_dimension() << "\n";
        tsp_file << "EDGE_WEIGHT_TYPE : EUC_2D\n";
        tsp_file << "NODE_COORD_SECTION\n";
        for(int i=0;i<test.get_dimension();i++)
        {
            tsp_file << i+1 << " " << test.get_city_coords()[i].x << " " << test.get_city_coords()[i].y << "\n";
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

    ifstream inputFile("output_tour.txt");
    vector<int> tour;
    int length = 0;

    // string line;
    bool tourSectionReached = false;

    while (getline(inputFile, line))
    {
        if (line.find("COMMENT : Length =") != string::npos)
        {
            // 提取路径长度
            string lengthStr = line.substr(line.find("=") + 1);
            length = stoi(lengthStr);
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
            tour.push_back(cityId);
        }
    }

    // output tour
    cerr << "Tour (size " << tour.size() << "): ";
    for (int cityId : tour)
    {
        cerr << cityId << " ";
    }
    cerr << endl;

    // output tour length
    cerr << "Length: " << length << endl;

    return 0;
}