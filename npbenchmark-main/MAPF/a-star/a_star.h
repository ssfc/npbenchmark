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

// Creating a shortcut for int, int pair type
using pos_pair = std::pair<int, int>;
// Creating a shortcut for pair<int, pair<int, int>> type
using pPair = std::pair<double, std::pair<int, int>>;

struct Coordinate
{
    int x;
    int y;
};

// A structure to hold the necessary parameters
struct cell
{
    // Row and Column index of its parent
    // Note that 0 <= i <= num_row-1 & 0 <= j <= num_columns-1
    int parent_i, parent_j;
    // f = g + h
    double f, g, h;
};


class AStar
{
private:
    int num_rows;
    int num_columns;
    int grid[9][10];
    pos_pair src;
    pos_pair dest;

    // Declare a 2D array of structure to hold the details of that cell
    cell cellDetails[9][10];
public:
    AStar(int input_num_rows, int input_num_columns, pos_pair src, pos_pair dest);

    // check whether given cell (row, col) is a valid cell or not.
    bool is_valid(pos_pair position) const;
    // check whether the given cell is blocked or not
    bool is_passable(pos_pair position);
    // check whether destination cell has been reached or not
    bool is_destination(pos_pair position) const;
    // calculate the 'h' heuristics.
    double calculate_h(pos_pair position) const;
    // trace the path from the source to destination
    void trace_path();
    // find the shortest path between a given source cell to a destination cell
    void a_star_search();
};

#endif //A_STAR_A_STAR_H