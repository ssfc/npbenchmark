// A C++ Program to implement A* Search Algorithm
// https://www.geeksforgeeks.org/a-search-algorithm/
#include <utility>
#include "a_star.h"

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

    // Source is the left-most bottom-most corner
    auto test_src = Coordinate{8, 0};

    // Destination is the left-most top-most corner
    // auto test_dest = Coordinate{0, 0};
    auto test_dest = Coordinate{2, 5};

    AStar test(test_src, test_dest);
    // test.output_map();
    test.a_star_search();

    return 0;
}