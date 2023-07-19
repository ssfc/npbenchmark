// A C++ Program to implement A* Search Algorithm
// https://www.geeksforgeeks.org/a-search-algorithm/
#include <utility>
#include "a_star.h"

using namespace std;


// Driver program to test above function
int main()
{
    /* Description of the Grid-
    1--> The cell is not blocked
    0--> The cell is blocked */
    std::vector<std::vector<int>>  test_grid
            = { { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
                { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 },
                { 1, 1, 1, 0, 1, 1, 0, 1, 0, 1 },
                { 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
                { 1, 1, 1, 0, 1, 1, 1, 0, 1, 0 },
                { 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 },
                { 1, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
                { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
                { 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 } };

    // Source is the left-most bottom-most corner
    pos_pair test_src = make_pair(8, 0);

    // Destination is the left-most top-most corner
    pos_pair test_dest = make_pair(0, 0);

    AStar test(9, 10, test_src, test_dest);
    test.output_map();
    test.a_star_search();

    return 0;
}