//
// Created by take_ on 2023/6/1.
//
extern "C" int compute_tsp(); // 声明c文件中的函数原型

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "travel_thief.h"

using namespace std;

int main(int argc, char* argv[])
{
    TravelThief travel_thief(argv[1], 600);
    // travel_thief.simple_heuristic();
    // travel_thief.random_local_search();
    // travel_thief.save_result();
    travel_thief.compute_object_value_by_saved_result(argv[2]);

    return 0;
}