//
// Created by take_ on 2023/7/17.
//
#include "a_star.h"

using namespace std;

AStar::AStar(Coordinate input_src, Coordinate input_dest):
        src(input_src),
        dest(input_dest)
// 0表示无障碍, 1表示有障碍。
{
    ifstream map_file("map.txt");
    if (map_file.is_open())
    {
        map_file >> num_rows >> num_columns;

        map.resize(num_rows);
        for(int i=0;i<num_rows;i++)
        {
            map[i].resize(num_columns);
        }

        for (int i = 0; i < num_rows; i++)
        {
            for (int j = 0; j < num_columns; j++)
            {
                char c;
                map_file >> c;
                if(c=='@')
                {
                    map[i][j] = 0;
                }
                else if(c=='.')
                {
                    map[i][j] = 1;
                }
            }
        }

        map_file.close();
    }
    else
    {
        cerr << "Error opening file." << endl;
    }

    cell_details.resize(num_rows);
    for(int i=0;i<num_rows;i++)
    {
        cell_details[i].resize(num_columns);
    }
}

// check whether given cell (row, col) is a valid cell or not.
// 也就是检查一个cell是否在地图范围内
bool AStar::is_valid(Coordinate position) const
{
    // Returns true if row number and column number is in range
    return (position.x >= 0) && (position.x < num_rows)
           && (position.y >= 0) && (position.y < num_columns);
}

// check whether the given cell is blocked or not
bool AStar::is_passable(Coordinate position)
{
    // Returns true if the cell is not blocked else false
    if (map[position.x][position.y] == 1)
        return true;
    else
        return false;
}

// check whether destination cell has been reached or not
bool AStar::is_destination(Coordinate position) const
{
    if (position.x == dest.x && position.y == dest.y)
        return true;
    else
        return false;
}

// calculate the 'h' heuristics.
double AStar::calculate_h(Coordinate position) const
{
    // Return using the distance formula
    // 看样子用的是欧几里得距离。
    return ((double)sqrt(
            (position.x - dest.x) * (position.x - dest.x)
            + (position.y - dest.y) * (position.y - dest.y)));
}

// trace the path from the source to destination
void AStar::trace_path()
{
    cerr << "\nThe Path is ";
    // 首先，函数 trace_path 初始化了起始位置的行索引 row 和列索引 col.
    int row = dest.x;
    int col = dest.y;

    // 接着创建了一个 stack 数据结构 Path 用于存储找到的最优路径.
    stack<Coordinate> Path;

    // 然后，通过一个 while 循环不断追踪回到起点，每次将当前位置 (row, col) 加入到 Path 中，然后将当前位置更新为其父节点的位置。
    // 这个过程会一直进行直到回到起点，即当前位置的父节点位置与其自身位置相同。
    // Comment: 看来这个程序的设定是起点的父就是自己,我的话倾向于设为-1。
    while (!(cell_details[row][col].parent_i == row
             && cell_details[row][col].parent_j == col))
    {
        Path.emplace(Coordinate{row, col});
        int temp_row = cell_details[row][col].parent_i;
        int temp_col = cell_details[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    // 接下来，将起点位置 (row, col) 也加入到 Path 中，此时 Path 中存储了完整的反向路径。
    Path.emplace(Coordinate{row, col});

    // 最后，通过另一个 while 循环遍历 Path 并依次输出路径上的每个坐标 (row, col)，形式为 -> (row,col)。
    while (!Path.empty())
    {
        Coordinate p = Path.top();
        Path.pop();
        cout << "-> (" << p.x << "," << p.y << ") ";
    }
}

// find the shortest path between a given source cell to a destination cell
void AStar::a_star_search()
{
    // Check whether the source is out of range
    if (!is_valid(src))
    {
        cerr <<"Source is invalid\n";
        return;
    }

    // Check whether the destination is out of range
    if (!is_valid(dest))
    {
        cerr << "Destination is invalid\n";
        return;
    }

    // Check whether the source or the destination is blocked
    if (!is_passable(src) || !is_passable(dest))
    {
        cerr << "Source or the destination is blocked\n";
        return;
    }

    // Check whether the destination cell is the same as source cell
    if (is_destination(src))
    {
        cerr << "We are already at the destination\n";
        return;
    }

    // Create a closed list and initialise it to false which means that no cell has been included yet
    // This closed list is implemented as a boolean 2D array
    bool closed_list[num_rows][num_columns];
    memset(closed_list, false, sizeof(closed_list));

    int i, j;

    for (i = 0; i < num_rows; i++)
    {
        for (j = 0; j < num_columns; j++)
        {
            cell_details[i][j].f = FLT_MAX;
            cell_details[i][j].g = FLT_MAX;
            cell_details[i][j].h = FLT_MAX;
            cell_details[i][j].parent_i = -1;
            cell_details[i][j].parent_j = -1;
        }
    }

    // Initialising the parameters of the starting node
    i = src.x, j = src.y;
    cell_details[i][j].f = 0.0;
    cell_details[i][j].g = 0.0;
    cell_details[i][j].h = 0.0;
    cell_details[i][j].parent_i = i;
    cell_details[i][j].parent_j = j;

    /*
    Create an open list having information as <f, <i, j>> where f = g + h,
    and i, j are the row and column index of that cell
    Note that 0 <= i <= num_row-1 & 0 <= j <= num_column-1
    This open list is implemented as a set of pair.*/
    set<pPair> open_list;

    // Put the starting cell on the open list and set its 'f' as 0
    open_list.insert(make_pair(0.0, make_pair(i, j)));

    // We set this boolean value as false as initially the destination is not reached.
    bool foundDest = false;

    while (!open_list.empty())
    {
        pPair open_begin = *open_list.begin();

        // Remove this vertex from the open list
        open_list.erase(open_list.begin());

        // Add this vertex to the closed list
        i = open_begin.second.first;
        j = open_begin.second.second;
        closed_list[i][j] = true;

        /*
        Generating all the 4 successor of this cell

                   N
                   |
            W----Cell----E
                  |
                  S

        Cell-->Popped Cell (i, j)
        N --> North	 (i-1, j)
        S --> South	 (i+1, j)
        E --> East	 (i, j+1)
        W --> West		 (i, j-1)
         */

        // To store the 'g', 'h' and 'f' of the 8 successors
        double gNew, hNew, fNew;

        //----------- 1st Successor (North) ------------

        // Only process this cell if this is a valid one
        if (is_valid(Coordinate{i-1, j}))
        {
            // If the destination cell is the same as the
            // current successor
            if (is_destination(Coordinate{i-1, j}))
            {
                // Set the Parent of the destination cell
                cell_details[i - 1][j].parent_i = i;
                cell_details[i - 1][j].parent_j = j;
                cerr << "The destination cell is found\n";
                trace_path();
                foundDest = true;
                return;
            }

                // If the successor is already on the closed list or if it is blocked, then ignore it.
                // Else do the following
            else if (!closed_list[i - 1][j] && is_passable(Coordinate{i - 1, j}))
            {
                gNew = cell_details[i][j].g + 1.0;
                hNew = calculate_h(Coordinate{i-1, j});
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cell_details[i - 1][j].f == FLT_MAX
                    || cell_details[i - 1][j].f > fNew) {
                    open_list.insert(make_pair(
                            fNew, make_pair(i - 1, j)));

                    // Update the details of this cell
                    cell_details[i - 1][j].f = fNew;
                    cell_details[i - 1][j].g = gNew;
                    cell_details[i - 1][j].h = hNew;
                    cell_details[i - 1][j].parent_i = i;
                    cell_details[i - 1][j].parent_j = j;
                }
            }
        }

        //----------- 2nd Successor (South) ------------

        // Only process this cell if this is a valid one
        if (is_valid(Coordinate{i+1, j}))
        {
            // If the destination cell is the same as the
            // current successor
            if (is_destination(Coordinate{i+1, j}))
            {
                // Set the Parent of the destination cell
                cell_details[i + 1][j].parent_i = i;
                cell_details[i + 1][j].parent_j = j;
                cerr << "The destination cell is found\n";
                trace_path();
                foundDest = true;

                return;
            }
                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (!closed_list[i + 1][j] && is_passable(Coordinate{i + 1, j}))
            {
                gNew = cell_details[i][j].g + 1.0;
                hNew = calculate_h(Coordinate{i+1, j});
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cell_details[i + 1][j].f == FLT_MAX
                    || cell_details[i + 1][j].f > fNew) {
                    open_list.insert(make_pair(
                            fNew, make_pair(i + 1, j)));
                    // Update the details of this cell
                    cell_details[i + 1][j].f = fNew;
                    cell_details[i + 1][j].g = gNew;
                    cell_details[i + 1][j].h = hNew;
                    cell_details[i + 1][j].parent_i = i;
                    cell_details[i + 1][j].parent_j = j;
                }
            }
        }

        //----------- 3rd Successor (East) ------------

        // Only process this cell if this is a valid one
        if (is_valid(Coordinate{i, j+1}))
        {
            // If the destination cell is the same as the
            // current successor
            if (is_destination(Coordinate{i, j+1}))
            {
                // Set the Parent of the destination cell
                cell_details[i][j + 1].parent_i = i;
                cell_details[i][j + 1].parent_j = j;
                cerr << "The destination cell is found\n";
                trace_path();
                foundDest = true;
                return;
            }

                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (!closed_list[i][j + 1] && is_passable(Coordinate{i, j + 1}))
            {
                gNew = cell_details[i][j].g + 1.0;
                hNew = calculate_h(Coordinate{i, j+1});
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cell_details[i][j + 1].f == FLT_MAX
                    || cell_details[i][j + 1].f > fNew) {
                    open_list.insert(make_pair(
                            fNew, make_pair(i, j + 1)));

                    // Update the details of this cell
                    cell_details[i][j + 1].f = fNew;
                    cell_details[i][j + 1].g = gNew;
                    cell_details[i][j + 1].h = hNew;
                    cell_details[i][j + 1].parent_i = i;
                    cell_details[i][j + 1].parent_j = j;
                }
            }
        }

        //----------- 4th Successor (West) ------------

        // Only process this cell if this is a valid one
        if (is_valid(Coordinate{i, j-1}))
        {
            // If the destination cell is the same as the
            // current successor
            if (is_destination(Coordinate{i, j-1}))
            {
                // Set the Parent of the destination cell
                cell_details[i][j - 1].parent_i = i;
                cell_details[i][j - 1].parent_j = j;
                cerr << "The destination cell is found\n";
                trace_path();
                foundDest = true;
                return;
            }

                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (!closed_list[i][j - 1] && is_passable(Coordinate{i, j - 1}))
            {
                gNew = cell_details[i][j].g + 1.0;
                hNew = calculate_h(Coordinate{i, j-1});
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cell_details[i][j - 1].f == FLT_MAX
                    || cell_details[i][j - 1].f > fNew) {
                    open_list.insert(make_pair(
                            fNew, make_pair(i, j - 1)));

                    // Update the details of this cell
                    cell_details[i][j - 1].f = fNew;
                    cell_details[i][j - 1].g = gNew;
                    cell_details[i][j - 1].h = hNew;
                    cell_details[i][j - 1].parent_i = i;
                    cell_details[i][j - 1].parent_j = j;
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


void AStar::output_map()
{
    ofstream map_file("map.txt");
    if (map_file.is_open())
    {
        map_file << num_rows << " " << num_columns << "\n";
        cerr << "Parameter file created successfully.\n";

        for(int i=0;i<num_rows;i++)
        {
            for(int j=0;j<num_columns;j++)
            {
                if(map[i][j]==0)
                {
                    map_file << '@';
                }
                else if(map[i][j]==1)
                {
                    map_file << '.';
                }

                if(j == num_columns-1)
                {
                    map_file << "\n";
                }
            }
        }

        map_file.close();
    }
    else
    {
        cerr << "Unable to create parameter file.\n";
    }
}