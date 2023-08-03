// A C++ Program to implement A* Search Algorithm
// https://www.geeksforgeeks.org/a-search-algorithm/
// pseudocode at : https://en.wikipedia.org/wiki/A*_search_algorithm
#include "a_star.h"
#include "cbs.h"

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

    /*
    // Source is the left-most bottom-most corner
    auto test_src = Coordinate{8, 0};

    // Destination is the left-most top-most corner
    auto test_dest = Coordinate{2, 4};
    // auto test_dest = Coordinate{2, 5};

    AStar test_astar(test_src, test_dest);
    // test_astar.output_map();
    test_astar.a_star_search();
     */

    CBS test_cbs;
    test_cbs.print_map();

    return 0;
}