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
            cout << "PROBLEM NAME:" << test.get_problem_name() << endl;
        }
        else if (line.find("KNAPSACK DATA TYPE:") != string::npos)
        {
            line.erase(0, 20); // 删除"KNAPSACK DATA TYPE: "，保留背包数据类型
            test.set_knapsack_data_type(line);
            cout << "KNAPSACK DATA TYPE:" << test.get_knapsack_data_type() << endl;
        }
        else if (line.find("DIMENSION:") != string::npos)
        {
            line.erase(0, 11); // 删除"DIMENSION: "，保留维度
            test.set_dimension(stoi(line));
            cout << "DIMENSION:" << test.get_dimension() << endl;
        }
        else if (line.find("NUMBER OF ITEMS:") != string::npos)
        {
            line.erase(0, 17); // 删除"NUMBER OF ITEMS: "，保留物品数量
            test.set_num_items(stoi(line));
            cout << "NUMBER OF ITEMS:" << test.get_num_items() << endl;
        }
        else if (line.find("CAPACITY OF KNAPSACK:") != string::npos)
        {
            line.erase(0, 22); // 删除"CAPACITY OF KNAPSACK: "，保留背包容量
            test.set_capacity_of_knapsack(stoi(line));
            cout << "CAPACITY OF KNAPSACK:" << test.get_capacity_of_knapsack() << endl;
        }
        else if (line.find("MIN SPEED:") != string::npos)
        {
            // cout << "MIN SPEED find" << endl;
            line.erase(0, 11); // 删除"MIN SPEED: "，保留最小速度
            test.set_min_speed(stod(line));
            cout << "MIN SPEED:" << test.get_min_speed() << endl;
        }
        else if (line.find("MAX SPEED:") != string::npos)
        {
            // cout << "MAX SPEED find" << endl;
            line.erase(0, 11); // 删除"MAX SPEED: "，保留最大速度
            test.set_max_speed(stod(line));
            cout << "MAX SPEED:" << test.get_max_speed() << endl;
        }
        else if (line.find("RENTING RATIO:") != string::npos)
        {
            line.erase(0, 14); // 删除"RENTING RATIO: "，保留租金比率
            test.set_renting_ratio(stod(line));
            cout << "RENTING RATIO:" << test.get_renting_ratio() << endl;
        }
        else if (line.find("EDGE_WEIGHT_TYPE:") != string::npos)
        {
            line.erase(0, 18); // 删除"EDGE_WEIGHT_TYPE: "，保留边权类型
            test.set_edge_weight_type(line);
            cout << "EDGE_WEIGHT_TYPE:" << test.get_edge_weight_type() << endl;
        }
        else if (line.find("NODE_COORD_SECTION") != string::npos)
        {
            cout << "NODE_COORD_SECTION (INDEX, X, Y): " << endl;
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

                    // cout << index << " " << temp.x << " " << temp.y << endl; // 输出节点坐标
                    test_num_node_cord++;
                }
                else
                {
                    cout << "ITEMS SECTION (INDEX, PROFIT, WEIGHT, ASSIGNED NODE NUMBER): " << endl;
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
                        // cout << index << " " << profit << " " << weight << " " << assigned_node << endl;
                        test_num_item_section++;
                    }
                }
            }
        }
    }

    file.close();
    cout << "test_num_node_cord: " << test_num_node_cord << endl;
    cout << "test_num_item_section: " << test_num_item_section << endl;
    return 0;
}
