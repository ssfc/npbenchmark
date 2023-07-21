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
        cell_details[i].resize(num_columns,
                               Cell{INT_MAX, INT_MAX, INT_MAX, -1, -1});
    }
}

// check whether given Cell (row, col) is a valid Cell or not.
// 也就是检查一个cell是否在地图范围内
bool AStar::is_valid(Coordinate position) const
{
    // Returns true if row number and column number is in range
    return (position.x >= 0) && (position.x < num_rows)
           && (position.y >= 0) && (position.y < num_columns);
}

// check whether the given Cell is blocked or not
bool AStar::is_passable(Coordinate position)
{
    // Returns true if the Cell is not blocked else false
    if (map[position.x][position.y] == 1)
        return true;
    else
        return false;
}

// check whether destination Cell has been reached or not
bool AStar::is_destination(Coordinate position) const
{
    if (position.x == dest.x && position.y == dest.y)
        return true;
    else
        return false;
}

// calculate the 'h' heuristics.
int AStar::calculate_h(Coordinate position) const
{
    // Return using the distance formula
    // 看样子用的是欧几里得距离。
    /*
    return ((double)sqrt(
            (position.x - dest.x) * (position.x - dest.x)
            + (position.y - dest.y) * (position.y - dest.y)));
*/
    // try manhattan distance
    return (abs(position.x - dest.x)
            + abs(position.y - dest.y));
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
    ofstream path_file("path.txt");
    if (path_file.is_open())
    {
        cerr << "total cost: " << Path.size() << endl;
        path_file << Path.size() << "\n";
        while (!Path.empty())
        {
            Coordinate position = Path.top();
            Path.pop();
            cout << "-> (" << position.x << "," << position.y << ") ";
            path_file << position.x << " " << position.y << "\n";
        }
    }
    else
    {
        cerr << "Unable to create path file.\n";
    }
}

// find the shortest path between a given source Cell to a destination Cell
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

    // Check whether the destination Cell is the same as source Cell
    if (is_destination(src))
    {
        cerr << "We are already at the destination\n";
        return;
    }

    // Create a closed list and initialise it to false which means that no Cell has been included yet
    // This closed list is implemented as a boolean 2D array
    // closed list是bool二维数组, open list是set, 有点诡异。
    bool closed_list[num_rows][num_columns];
    memset(closed_list, false, sizeof(closed_list));
    bool open_table[num_rows][num_columns];
    memset(open_table, false, sizeof(open_table));

    // Initialising the parameters of the starting node
    cell_details[src.x][src.y] = Cell{0.0, 0.0, 0.0, src.x, src.y};

    /*
    Create an open list having information as <f, <i, j>> where f = g + h,
    and i, j are the row and column index of that Cell
    Note that 0 <= i <= num_row-1 & 0 <= j <= num_column-1
    This open list is implemented as a set of pair.*/
    set<OpenNode> open_list;

    // Put the starting Cell on the open list and set its 'f' as 0
    open_list.insert(OpenNode{0.0, Coordinate{src.x, src.y}});
    open_table[src.x][src.y] = true;

    // We set this boolean value as false as initially the destination is not reached.
    bool found_dest = false;

    while (!open_list.empty())
    {
        OpenNode open_begin = *open_list.begin();

        // Remove this vertex from the open list
        open_list.erase(open_list.begin());

        // Add this vertex to the closed list
        int current_x = open_begin.position.x;
        int current_y = open_begin.position.y;
        closed_list[current_x][current_y] = true;
        open_table[current_x][current_y] = false;

        /*
        Generating all the 4 successor of this Cell

                   N
                   |
            W----Cell----E
                  |
                  S

        Cell-->Popped Cell (current_x, current_y)
        N --> North	 (current_x, current_y+1)
        S --> South	 (current_x, current_y-1)
        W --> West	 (current_x-1, current_y)
        E --> East	 (current_x+1, current_y)
         */

        // To store the 'g', 'h' and 'f' of the 8 successors
        double g_new, h_new, f_new;

        //----------- 1st Successor (North) ------------
        int north_x = current_x;
        int north_y = current_y + 1;
        // Only process this Cell if this is a valid one
        if (is_valid(Coordinate{north_x, north_y}))
        {
            // If the destination Cell is the same as the current successor
            if (is_destination(Coordinate{north_x, north_y}))
            {
                // Set the Parent of the destination Cell
                cell_details[north_x][north_y].parent_i = current_x;
                cell_details[north_x][north_y].parent_j = current_y;
                cerr << "The destination Cell is found\n";
                trace_path();
                found_dest = true;

                return;
            }
            // If the successor has not been evaluated and is passable
            else if (!closed_list[north_x][north_y] && is_passable(Coordinate{north_x, north_y}))
            {
                g_new = cell_details[current_x][current_y].g + 1.0;
                h_new = calculate_h(Coordinate{north_x, north_y});
                f_new = g_new + h_new;

                // Make the current square the parent of this square.
                // Record the f, g, and h costs of the square Cell
                //			 OR
                // If it is on the open list already, check to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cell_details[north_x][north_y].f > f_new)
                {
                    // Update the details of this Cell
                    cell_details[north_x][north_y] = Cell{f_new, g_new, h_new,
                                                          current_x, current_y};
                    // If it isn’t on the open list, add it to the open list.
                    if(!open_table[north_x][north_y])
                    {
                        open_table[north_x][north_y] = true;
                        open_list.insert(OpenNode{f_new, Coordinate{north_x, north_y}});
                    }
                }
            }
        }

        //----------- 2nd Successor (South) ------------
        int south_x = current_x;
        int south_y = current_y - 1;
        // Only process this Cell if it is valid
        if (is_valid(Coordinate{south_x, south_y}))
        {
            // If the destination Cell is the same as the current successor
            if (is_destination(Coordinate{south_x, south_y}))
            {
                // Set the Parent of the destination Cell
                cell_details[south_x][south_y].parent_i = current_x;
                cell_details[south_x][south_y].parent_j = current_y;
                cerr << "The destination Cell is found\n";
                trace_path();
                found_dest = true;

                return;
            }
            // If the successor has not been evaluated and is passable
            else if (!closed_list[south_x][south_y] && is_passable(Coordinate{south_x, south_y}))
            {
                g_new = cell_details[current_x][current_y].g + 1.0;
                h_new = calculate_h(Coordinate{south_x, south_y});
                f_new = g_new + h_new;

                // Make the current square the parent of this square.
                // Record the f, g, and h costs of the square Cell
                //			 OR
                // If it is on the open list already, check to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cell_details[south_x][south_y].f > f_new)
                {
                    // Update the details of this Cell
                    cell_details[south_x][south_y] = Cell{f_new, g_new, h_new, current_x, current_y};
                    // If it isn’t on the open list, add it to the open list.
                    if(!open_table[south_x][south_y])
                    {
                        open_table[south_x][south_y] = true;
                        open_list.insert(OpenNode{f_new, Coordinate{south_x, south_y}});
                    }
                }
            }
        }

        //----------- 3rd Successor (West) ------------
        int west_x = current_x - 1;
        int west_y = current_y;
        // Only process this Cell if this is a valid one
        if (is_valid(Coordinate{west_x, west_y}))
        {
            // If the destination Cell is the same as the current successor
            if (is_destination(Coordinate{west_x, west_y}))
            {
                // Set the Parent of the destination Cell
                cell_details[west_x][west_y].parent_i = current_x;
                cell_details[west_x][west_y].parent_j = current_y;
                cerr << "The destination Cell is found\n";
                trace_path();
                found_dest = true;

                return;
            }
            // If the successor has not been evaluated and is passable
            else if (!closed_list[west_x][west_y] && is_passable(Coordinate{west_x, west_y}))
            {
                g_new = cell_details[current_x][current_y].g + 1.0;
                h_new = calculate_h(Coordinate{west_x, west_y});
                f_new = g_new + h_new;

                // Make the current square the parent of this square.
                // Record the f, g, and h costs of the square Cell
                //			 OR
                // If it is on the open list already, check to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cell_details[west_x][west_y].f > f_new)
                {
                    // Update the details of this Cell
                    cell_details[west_x][west_y] = Cell{f_new, g_new, h_new,
                                                        current_x, current_y};
                    // If it isn’t on the open list, add it to the open list.
                    if(!open_table[west_x][west_y])
                    {
                        open_table[west_x][west_y] = true;
                        open_list.insert(OpenNode{f_new, Coordinate{west_x, west_y}});
                    }
                }
            }
        }

        //----------- 4th Successor (East) ------------
        int east_x = current_x + 1;
        int east_y = current_y;
        // Only process this Cell if this is a valid one
        if (is_valid(Coordinate{east_x, east_y}))
        {
            // If the destination Cell is the same as the current successor
            if (is_destination(Coordinate{east_x, east_y}))
            {
                // Set the Parent of the destination Cell
                cell_details[east_x][east_y].parent_i = current_x;
                cell_details[east_x][east_y].parent_j = current_y;
                cerr << "The destination Cell is found\n";
                trace_path();
                found_dest = true;

                return;
            }
            // If the successor has not been evaluated and is passable
            else if (!closed_list[east_x][east_y] && is_passable(Coordinate{east_x, east_y}))
            {
                g_new = cell_details[current_x][current_y].g + 1.0;
                h_new = calculate_h(Coordinate{east_x, east_y});
                f_new = g_new + h_new;

                // Make the current square the parent of this square.
                // Record the f, g, and h costs of the square Cell
                //			 OR
                // If it is on the open list already, check to see if this path to that square is better,
                // using 'f' cost as the measure.
                if (cell_details[east_x][east_y].f > f_new)
                {
                    // Update the details of this Cell
                    cell_details[east_x][east_y] = Cell{f_new, g_new, h_new,
                                                        current_x, current_y};
                    // If it isn’t on the open list, add it to the open list.
                    if(!open_table[east_x][east_y])
                    {
                        open_table[east_x][east_y] = true;
                        open_list.insert(OpenNode{f_new, Coordinate{east_x, east_y}});
                    }
                }
            }
        }

    }

    // When the destination Cell is not found and the open list is empty,
    // then we conclude that we failed to reach the destination Cell.
    // This may happen when there is no way to destination Cell (due to blockages)
    if (!found_dest)
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
        cerr << "Unable to create map file.\n";
    }
}