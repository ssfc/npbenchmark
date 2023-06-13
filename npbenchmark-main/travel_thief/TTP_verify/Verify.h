//
// Created by Star on 2023/6/13.
//

#ifndef VERIFY_VERIFY_H
#define VERIFY_VERIFY_H

#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <map>
#include <fstream>

using namespace std;
typedef pair<int, int> PII;

const int N = 40010, M = 350010;
const int INF = 0x3f3f3f3f;

struct CITY
{
    int x;
    int y;
    vector<int> item_list;
};

struct ITEM
{
    int id;
    int val;
    int weight;
    int pos;
};

void read_data(istream &fin);
void read_solve(istream &fin);
double get_dis(CITY &a,CITY &b);
void pre_cal_dis();
double cal_val();

#endif //VERIFY_VERIFY_H
