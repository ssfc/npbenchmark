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

int main()
{
    TravelThief travel_thief;
    // travel_thief.simple_heuristic();
    travel_thief.random_local_search();

    return 0;
}