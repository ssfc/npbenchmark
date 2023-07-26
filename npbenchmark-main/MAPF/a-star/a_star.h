//
// Created by take_ on 2023/7/17.
//

#ifndef A_STAR_A_STAR_H
#define A_STAR_A_STAR_H

#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <set>
#include <stack>
#include <utility>
#include <vector>

struct Coordinate
{
    int x;
    int y;
};

struct OpenNode
{
    int heuristic_cost;
    Coordinate position;

    bool operator<(const OpenNode& other) const
    {
        if (heuristic_cost != other.heuristic_cost)
        {
            return heuristic_cost < other.heuristic_cost;
        }
        else
        {
            if (position.x != other.position.x)
            {
                return position.x < other.position.x;
            }
            else
            {
                return position.y < other.position.y;
            }
        }
    }
};

// A structure to hold the necessary parameters
struct Cell
{
    int g_score; // cost of the cheapest path from start to n currently known
    int h; // h(n) estimates the cost to reach goal from node n
    int f; // f = g_score + h; fScore[n] represents our current best guess as to
    // how cheap a path could be from start to finish if it goes through n.

    // Row and Column index of its parent
    // Note that 0 <= i <= num_row-1 & 0 <= j <= num_columns-1
    Coordinate parent;
};


class AStar
{
private:
    int num_rows;
    int num_columns;
    std::vector<std::vector<int>> map;
    Coordinate src;
    Coordinate dest;

    // Declare a 2D array of structure to hold the details of that Cell
    std::vector<std::vector<Cell>> cell_details;

    std::vector<std::vector<int>> open_list;
    std::vector<std::vector<int>> closed_list;

    // debug var;
    int iter;
    double start_time;
public:
    AStar(Coordinate input_src, Coordinate input_dest);

    // check whether given Cell (row, col) is a valid Cell or not.
    [[nodiscard]] bool is_valid(Coordinate position) const;
    // check whether the given Cell is blocked or not
    bool is_passable(Coordinate position);
    // check whether destination Cell has been reached or not
    [[nodiscard]] bool is_destination(Coordinate position) const;
    // calculate the 'h' heuristics.
    [[nodiscard]] int calculate_h(Coordinate position) const;
    // trace the path from the source to destination
    void trace_path();
    // find the shortest path between a given source Cell to a destination Cell
    void a_star_search();

    // debug func
    void output_map();
    void print_open_list();
    void print_closed_list();
};

#endif //A_STAR_A_STAR_H