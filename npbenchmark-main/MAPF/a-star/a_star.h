//
// Created by take_ on 2023/7/17.
//

#ifndef A_STAR_A_STAR_H
#define A_STAR_A_STAR_H

#include <cfloat>
#include <cmath>
#include <cstring>
#include <iostream>
#include <set>
#include <stack>
#include <utility>

using namespace std;

// Creating a shortcut for int, int pair type
using Coordinate = pair<int, int>;
// Creating a shortcut for pair<int, pair<int, int>> type
using pPair = pair<double, pair<int, int>>;

// A structure to hold the necessary parameters
struct cell
{
    // Row and Column index of its parent
    // Note that 0 <= i <= num_row-1 & 0 <= j <= num_columns-1
    int parent_i, parent_j;
    // f = g + h
    double f, g, h;
};





#endif //A_STAR_A_STAR_H
