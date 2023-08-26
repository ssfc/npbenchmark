// A C++ Program to implement A* Search Algorithm
// https://www.geeksforgeeks.org/a-search-algorithm/
// pseudocode at : https://en.wikipedia.org/wiki/A*_search_algorithm
#include "a_star.h"
#include "cbs.h"
#include "low_level.h"

using namespace std;


// Driver program to test above function
int main()
{
    /* Description of the Grid-
    1--> The Cell is not blocked
    0--> The Cell is blocked */
    /*
    std::vector<std::vector<int>> test_map
            = { { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
                { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 },
                { 1, 1, 1, 0, 1, 1, 0, 1, 0, 1 },
                { 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
                { 1, 1, 1, 0, 1, 1, 1, 0, 1, 0 },
                { 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 },
                { 1, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
                { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
                { 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 } };
                */

    ///*
    // Source is the left-most bottom-most corner
    auto test_src = Coordinate{8, 0};

    // Destination is the left-most top-most corner
    auto test_dest = Coordinate{2, 4};
    // auto test_dest = Coordinate{2, 5};

    AStar test_astar(test_src, test_dest);
    // test_astar.output_map();
    test_astar.a_star_search();
     //*/

    /*
    CBS test_cbs;
    test_cbs.print_map();
    // test_cbs.find_solution();

    LowLevel test_low_level(test_cbs.get_map(), test_cbs.get_agents());
    test_low_level.print_map();
     */

#include <iostream>
#include <queue>

    struct Coordinate {
        int x, y;
    };

    struct OpenNode {
        int f_score;
        Coordinate position;

        bool operator<(const OpenNode& other) const {
            if (f_score != other.f_score) {
                return f_score > other.f_score;  // 注意这里使用 > 符号
            } else {
                if (position.x != other.position.x) {
                    return position.x > other.position.x;
                } else {
                    return position.y > other.position.y;
                }
            }
        }
    };

/*
    std::priority_queue<OpenNode> open_queue;

    OpenNode node1 = {10, {2, 3}};
    OpenNode node2 = {8, {1, 2}};

    open_queue.push(node1);
    open_queue.push(node2);

    cerr << endl;
    while (!open_queue.empty()) {
        OpenNode top_node = open_queue.top();
        open_queue.pop();
        cerr << "f_score: " << top_node.f_score << ", position: (" << top_node.position.x << ", "
        << top_node.position.y << ")" << endl;
    }
*/


    return 0;
}