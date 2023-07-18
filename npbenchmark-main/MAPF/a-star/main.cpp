// A C++ Program to implement A* Search Algorithm
// https://www.geeksforgeeks.org/a-search-algorithm/
#include <cfloat>
#include <cmath>
#include <cstring>
#include <iostream>
#include <set>
#include <stack>
#include <utility>
#include "a_star.h"

using namespace std;


// Creating a shortcut for int, int pair type
using Pair = pair<int, int>;

// Creating a shortcut for pair<int, pair<int, int>> type
using pPair = pair<double, pair<int, int> >;

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

    // Declare a 2D array of structure to hold the details of that cell
    cell cellDetails[9][10];
public:
    AStar(int input_num_rows, int input_num_columns);

    // check whether given cell (row, col) is a valid cell or not.
    bool is_valid(int row, int col);
    // check whether the given cell is blocked or not
    bool is_unblocked(int row, int col);
    // check whether destination cell has been reached or not
    static bool is_destination(int row, int col, Pair dest);
    // calculate the 'h' heuristics.
    static double calculate_h(int row, int col, Pair dest);
    // trace the path from the source to destination
    void trace_path(Pair dest);
    // find the shortest path between a given source cell to a destination cell
    void a_star_search(Pair src, Pair dest);
};

AStar::AStar(int input_num_rows, int input_num_columns):
num_rows(input_num_rows),
num_columns(input_num_columns),
grid{ { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
      { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 },
      { 1, 1, 1, 0, 1, 1, 0, 1, 0, 1 },
      { 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
      { 1, 1, 1, 0, 1, 1, 1, 0, 1, 0 },
      { 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 },
      { 1, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
      { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
      { 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 } }
{

}

// check whether given cell (row, col) is a valid cell or not.
bool AStar::is_valid(int row, int col)
{
    // Returns true if row number and column number is in range
    return (row >= 0) && (row < num_rows) && (col >= 0) && (col < num_columns);
}

// check whether the given cell is blocked or not
bool AStar::is_unblocked(int row, int col)
{
    // Returns true if the cell is not blocked else false
    if (grid[row][col] == 1)
        return true;
    else
        return false;
}

// check whether destination cell has been reached or not
bool AStar::is_destination(int row, int col, Pair dest)
{
    if (row == dest.first && col == dest.second)
        return true;
    else
        return false;
}

// calculate the 'h' heuristics.
double AStar::calculate_h(int row, int col, Pair dest)
{
    // Return using the distance formula
    return ((double)sqrt(
            (row - dest.first) * (row - dest.first)
            + (col - dest.second) * (col - dest.second)));
}

void AStar::trace_path(Pair dest)
{
    cerr << "\nThe Path is ";
    int row = dest.first;
    int col = dest.second;

    stack<Pair> Path;

    while (!(cellDetails[row][col].parent_i == row
             && cellDetails[row][col].parent_j == col))
    {
        Path.emplace(row, col);
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    Path.emplace(row, col);
    while (!Path.empty())
    {
        pair<int, int> p = Path.top();
        Path.pop();
        cout << "-> (" << p.first << "," << p.second << ") ";
    }
}

void AStar::a_star_search(Pair src, Pair dest)
{
    // If the source is out of range
    if (!is_valid(src.first, src.second))
    {
        cerr <<"Source is invalid\n";
        return;
    }

    // If the destination is out of range
    if (!is_valid(dest.first, dest.second))
    {
        cerr << "Destination is invalid\n";
        return;
    }

    // Either the source or the destination is blocked
    if (!is_unblocked(src.first, src.second)
        || !is_unblocked(dest.first, dest.second))
    {
        cerr << "Source or the destination is blocked\n";
        return;
    }

    // If the destination cell is the same as source cell
    if (is_destination(src.first, src.second, dest))
    {
        cerr << "We are already at the destination\n";
        return;
    }

    // Create a closed list and initialise it to false which
    // means that no cell has been included yet This closed
    // list is implemented as a boolean 2D array
    bool closedList[num_rows][num_columns];
    memset(closedList, false, sizeof(closedList));

    int i, j;

    for (i = 0; i < num_rows; i++)
    {
        for (j = 0; j < num_columns; j++)
        {
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }

    // Initialising the parameters of the starting node
    i = src.first, j = src.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;

    /*
    Create an open list having information as-
    <f, <i, j>>
    where f = g + h,
    and i, j are the row and column index of that cell
    Note that 0 <= i <= num_row-1 & 0 <= j <= num_column-1
    This open list is implemented as a set of pair of
    pair.*/
    set<pPair> openList;

    // Put the starting cell on the open list and set its
    // 'f' as 0
    openList.insert(make_pair(0.0, make_pair(i, j)));

    // We set this boolean value as false as initially
    // the destination is not reached.
    bool foundDest = false;

    while (!openList.empty())
    {
        pPair p = *openList.begin();

        // Remove this vertex from the open list
        openList.erase(openList.begin());

        // Add this vertex to the closed list
        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;

        /*
        Generating all the 8 successor of this cell

            N.W N N.E
            \ | /
                \ | /
            W----Cell----E
                / | \
                / | \
            S.W S S.E

        Cell-->Popped Cell (i, j)
        N --> North	 (i-1, j)
        S --> South	 (i+1, j)
        E --> East	 (i, j+1)
        W --> West		 (i, j-1)
        N.E--> North-East (i-1, j+1)
        N.W--> North-West (i-1, j-1)
        S.E--> South-East (i+1, j+1)
        S.W--> South-West (i+1, j-1)*/

        // To store the 'g', 'h' and 'f' of the 8 successors
        double gNew, hNew, fNew;

        //----------- 1st Successor (North) ------------

        // Only process this cell if this is a valid one
        if (is_valid(i - 1, j))
        {
            // If the destination cell is the same as the
            // current successor
            if (is_destination(i - 1, j, dest))
            {
                // Set the Parent of the destination cell
                cellDetails[i - 1][j].parent_i = i;
                cellDetails[i - 1][j].parent_j = j;
                cerr << "The destination cell is found\n";
                trace_path(dest);
                foundDest = true;
                return;
            }

            // If the successor is already on the closed list or if it is blocked, then ignore it.
            // Else do the following
            else if (!closedList[i - 1][j] && is_unblocked(i - 1, j))
            {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculate_h(i - 1, j, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i - 1][j].f == FLT_MAX
                    || cellDetails[i - 1][j].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(i - 1, j)));

                    // Update the details of this cell
                    cellDetails[i - 1][j].f = fNew;
                    cellDetails[i - 1][j].g = gNew;
                    cellDetails[i - 1][j].h = hNew;
                    cellDetails[i - 1][j].parent_i = i;
                    cellDetails[i - 1][j].parent_j = j;
                }
            }
        }

        //----------- 2nd Successor (South) ------------

        // Only process this cell if this is a valid one
        if (is_valid(i + 1, j))
        {
            // If the destination cell is the same as the
            // current successor
            if (is_destination(i + 1, j, dest))
            {
                // Set the Parent of the destination cell
                cellDetails[i + 1][j].parent_i = i;
                cellDetails[i + 1][j].parent_j = j;
                cerr << "The destination cell is found\n";
                trace_path(dest);
                foundDest = true;

                return;
            }
                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (!closedList[i + 1][j] && is_unblocked(i + 1, j))
            {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculate_h(i + 1, j, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i + 1][j].f == FLT_MAX
                    || cellDetails[i + 1][j].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(i + 1, j)));
                    // Update the details of this cell
                    cellDetails[i + 1][j].f = fNew;
                    cellDetails[i + 1][j].g = gNew;
                    cellDetails[i + 1][j].h = hNew;
                    cellDetails[i + 1][j].parent_i = i;
                    cellDetails[i + 1][j].parent_j = j;
                }
            }
        }

        //----------- 3rd Successor (East) ------------

        // Only process this cell if this is a valid one
        if (is_valid(i, j + 1))
        {
            // If the destination cell is the same as the
            // current successor
            if (is_destination(i, j + 1, dest))
            {
                // Set the Parent of the destination cell
                cellDetails[i][j + 1].parent_i = i;
                cellDetails[i][j + 1].parent_j = j;
                cerr << "The destination cell is found\n";
                trace_path(dest);
                foundDest = true;
                return;
            }

                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (!closedList[i][j + 1] && is_unblocked(i, j + 1))
            {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculate_h(i, j + 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i][j + 1].f == FLT_MAX
                    || cellDetails[i][j + 1].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(i, j + 1)));

                    // Update the details of this cell
                    cellDetails[i][j + 1].f = fNew;
                    cellDetails[i][j + 1].g = gNew;
                    cellDetails[i][j + 1].h = hNew;
                    cellDetails[i][j + 1].parent_i = i;
                    cellDetails[i][j + 1].parent_j = j;
                }
            }
        }

        //----------- 4th Successor (West) ------------

        // Only process this cell if this is a valid one
        if (is_valid(i, j - 1))
        {
            // If the destination cell is the same as the
            // current successor
            if (is_destination(i, j - 1, dest))
            {
                // Set the Parent of the destination cell
                cellDetails[i][j - 1].parent_i = i;
                cellDetails[i][j - 1].parent_j = j;
                cerr << "The destination cell is found\n";
                trace_path(dest);
                foundDest = true;
                return;
            }

                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (!closedList[i][j - 1]
                     && is_unblocked(i, j - 1))
            {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculate_h(i, j - 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i][j - 1].f == FLT_MAX
                    || cellDetails[i][j - 1].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(i, j - 1)));

                    // Update the details of this cell
                    cellDetails[i][j - 1].f = fNew;
                    cellDetails[i][j - 1].g = gNew;
                    cellDetails[i][j - 1].h = hNew;
                    cellDetails[i][j - 1].parent_i = i;
                    cellDetails[i][j - 1].parent_j = j;
                }
            }
        }

        //----------- 5th Successor (North-East)
        //------------

        // Only process this cell if this is a valid one
        if (is_valid(i - 1, j + 1))
        {
            // If the destination cell is the same as the
            // current successor
            if (is_destination(i - 1, j + 1, dest))
            {
                // Set the Parent of the destination cell
                cellDetails[i - 1][j + 1].parent_i = i;
                cellDetails[i - 1][j + 1].parent_j = j;
                cerr << "The destination cell is found\n";
                trace_path(dest);
                foundDest = true;

                return;
            }

                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (!closedList[i - 1][j + 1]
                     && is_unblocked(i - 1, j + 1))
            {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculate_h(i - 1, j + 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i - 1][j + 1].f == FLT_MAX
                    || cellDetails[i - 1][j + 1].f > fNew)
                {
                    openList.insert(make_pair(
                            fNew, make_pair(i - 1, j + 1)));

                    // Update the details of this cell
                    cellDetails[i - 1][j + 1].f = fNew;
                    cellDetails[i - 1][j + 1].g = gNew;
                    cellDetails[i - 1][j + 1].h = hNew;
                    cellDetails[i - 1][j + 1].parent_i = i;
                    cellDetails[i - 1][j + 1].parent_j = j;
                }
            }
        }

        //----------- 6th Successor (North-West)
        //------------

        // Only process this cell if this is a valid one
        if (is_valid(i - 1, j - 1))
        {
            // If the destination cell is the same as the
            // current successor
            if (is_destination(i - 1, j - 1, dest))
            {
                // Set the Parent of the destination cell
                cellDetails[i - 1][j - 1].parent_i = i;
                cellDetails[i - 1][j - 1].parent_j = j;
                cerr << "The destination cell is found\n";
                trace_path(dest);
                foundDest = true;

                return;
            }

                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (!closedList[i-1][j-1] && is_unblocked(i-1, j-1))
            {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculate_h(i - 1, j - 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i - 1][j - 1].f == FLT_MAX
                    || cellDetails[i - 1][j - 1].f > fNew)
                {
                    openList.insert(make_pair(
                            fNew, make_pair(i - 1, j - 1)));
                    // Update the details of this cell
                    cellDetails[i - 1][j - 1].f = fNew;
                    cellDetails[i - 1][j - 1].g = gNew;
                    cellDetails[i - 1][j - 1].h = hNew;
                    cellDetails[i - 1][j - 1].parent_i = i;
                    cellDetails[i - 1][j - 1].parent_j = j;
                }
            }
        }

        //----------- 7th Successor (South-East)
        //------------

        // Only process this cell if this is a valid one
        if (is_valid(i + 1, j + 1))
        {
            // If the destination cell is the same as the
            // current successor
            if (is_destination(i + 1, j + 1, dest))
            {
                // Set the Parent of the destination cell
                cellDetails[i + 1][j + 1].parent_i = i;
                cellDetails[i + 1][j + 1].parent_j = j;
                cerr << "The destination cell is found\n";
                trace_path(dest);
                foundDest = true;

                return;
            }

                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (!closedList[i+1][j+1] && is_unblocked(i+1, j+1))
            {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculate_h(i + 1, j + 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i + 1][j + 1].f == FLT_MAX
                    || cellDetails[i + 1][j + 1].f > fNew) {
                    openList.insert(make_pair(
                            fNew, make_pair(i + 1, j + 1)));

                    // Update the details of this cell
                    cellDetails[i + 1][j + 1].f = fNew;
                    cellDetails[i + 1][j + 1].g = gNew;
                    cellDetails[i + 1][j + 1].h = hNew;
                    cellDetails[i + 1][j + 1].parent_i = i;
                    cellDetails[i + 1][j + 1].parent_j = j;
                }
            }
        }

        //----------- 8th Successor (South-West)
        //------------

        // Only process this cell if this is a valid one
        if (is_valid(i + 1, j - 1))
        {
            // If the destination cell is the same as the
            // current successor
            if (is_destination(i + 1, j - 1, dest))
            {
                // Set the Parent of the destination cell
                cellDetails[i + 1][j - 1].parent_i = i;
                cellDetails[i + 1][j - 1].parent_j = j;
                cerr << "The destination cell is found\n";
                trace_path(dest);
                foundDest = true;

                return;
            }

                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (!closedList[i+1][j-1] && is_unblocked(i+1, j-1))
            {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculate_h(i + 1, j - 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i + 1][j - 1].f == FLT_MAX
                    || cellDetails[i + 1][j - 1].f > fNew)
                {
                    openList.insert(make_pair(
                            fNew, make_pair(i + 1, j - 1)));

                    // Update the details of this cell
                    cellDetails[i + 1][j - 1].f = fNew;
                    cellDetails[i + 1][j - 1].g = gNew;
                    cellDetails[i + 1][j - 1].h = hNew;
                    cellDetails[i + 1][j - 1].parent_i = i;
                    cellDetails[i + 1][j - 1].parent_j = j;
                }
            }
        }
    }

    // When the destination cell is not found and the open
    // list is empty, then we conclude that we failed to
    // reach the destination cell. This may happen when the
    // there is no way to destination cell (due to
    // blockages)
    if (!foundDest)
        cerr << "Failed to find the Destination Cell\n";
}

// Driver program to test above function
int main()
{
	/* Description of the Grid-
	1--> The cell is not blocked
	0--> The cell is blocked */
	int test_grid[9][10]
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
	Pair src = make_pair(8, 0);

	// Destination is the left-most top-most corner
	Pair dest = make_pair(0, 0);

    AStar test(9, 10);
	test.a_star_search(src, dest);

	return 0;
}
