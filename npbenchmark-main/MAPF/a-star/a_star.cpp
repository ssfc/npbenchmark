//
// Created by YG on 2023/7/17.
//
#include "a_star.h"

using namespace std;

AStar::AStar(Coordinate input_src, Coordinate input_dest):
        src(input_src),
        dest(input_dest),
        iter(0),
        start_time(clock())
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

        // initialize open list and closed list;
        open_list.resize(num_rows);
        for(int i=0;i<num_rows;i++)
        {
            open_list[i].resize(num_columns, 0);
        }

        closed_list.resize(num_rows);
        for(int i=0;i<num_rows;i++)
        {
            closed_list[i].resize(num_columns, 0);
        }
    }
    else
    {
        cerr << "Error opening file." << endl;
    }

    cell_details.resize(num_rows);
    for(int i=0;i<num_rows;i++)
    {
        cell_details[i].resize(num_columns,
                               Cell{INT_MAX, INT_MAX, Coordinate{-1, -1}});
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

// calculate the 'h_score' heuristics.
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

// A* LINE 1
// function reconstruct_path(cameFrom, current)
// cameFrom: 当前节点的前驱（或者说父节点）
// current: 当前节点
// meaning: 构造出agent path
void AStar::trace_path()
{
    double elapsed_time = (clock() - start_time) / CLOCKS_PER_SEC;
    cerr << "success, iterations: " << iter << " elapsed_time(s): " << elapsed_time
         << " frequency:" << double (iter) / elapsed_time << endl;

    // 首先，函数 trace_path 初始化了起始位置的行索引 row 和列索引 col.
    int row = dest.x;
    int col = dest.y;

    // A* LINE 2
    // total_path := {current}
    // current: 当前节点
    // total_path: 已经构造的路径
    // 接着创建了一个 stack 数据结构 Path 用于存储找到的最优路径.
    stack<Coordinate> Path;
    Path.emplace(Coordinate{row, col});

    // A* LINE 3
    // while current in cameFrom.Keys:
    // 然后，通过一个 while 循环不断追踪回到起点，每次将当前位置 (row, col) 加入到 Path 中，然后将当前位置更新为其父节点的位置。
    // 这个过程会一直进行直到回到起点，即当前位置的父节点位置与其自身位置相同。
    // Comment: 看来这个程序的设定是起点的父就是自己,我的话倾向于设为-1。
    while (!(cell_details[row][col].parent.x == row
             && cell_details[row][col].parent.y == col))
    {
        int temp_row = cell_details[row][col].parent.x;
        int temp_col = cell_details[row][col].parent.y;
        row = temp_row;
        col = temp_col;
        Path.emplace(Coordinate{row, col});
    }

    // 最后，通过另一个 while 循环遍历 Path 并依次输出路径上的每个坐标 (row, col)，形式为 -> (row,col)。
    ofstream path_file("path.txt");
    if (path_file.is_open())
    {
        cerr << "total cost: " << Path.size() << endl;
        path_file << Path.size() << "\n";
        cerr << "The Path is ";
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

// find the shortest path between a given source cell to a destination cell
bool AStar::a_star_search()
{
    // Check whether the source is out of range
    if (!is_valid(src))
    {
        cerr <<"Source is invalid\n";

        return false;
    }

    // Check whether the destination is out of range
    if (!is_valid(dest))
    {
        cerr << "Destination is invalid\n";

        return false;
    }

    // Check whether the source or the destination is blocked
    if (!is_passable(src) || !is_passable(dest))
    {
        cerr << "Source or the destination is blocked\n";

        return false;
    }

    // Check whether the destination cell is the same as source cell
    if (is_destination(src))
    {
        cerr << "We are already at the destination\n";

        return false;
    }

    // Create a closed list and initialise it to false which means that no cell has been included yet
    // This closed list is implemented as a boolean 2D array
    // closed list是bool二维数组, open list是set, 有点诡异。

    // Initialising the parameters of the starting node
    int f_start = calculate_h(src);
    cell_details[src.x][src.y] = Cell{0, f_start, src};

    /*
    Create an open list having information as <f_score, <i, j>> where f_score = g_score + h_score,
    and i, j are the row and column index of that cell
    Note that 0 <= i <= num_row-1 & 0 <= j <= num_column-1
    This open list is implemented as a set of pair.*/
    set<OpenNode> open_set;

    // Put the starting cell on the open list and set its 'f_score' as 0
    open_set.insert(OpenNode{f_start, src});
    open_list[src.x][src.y] = 1;

    cerr << "Add start to closed list" << endl;
    print_open_list(); // print_open_list();
    print_closed_list();

    while (!open_set.empty() && iter < INT_MAX)
    {
        cerr << "iter: " << iter << endl;
        Coordinate current = open_set.begin()->position;

        if (is_destination(current))
        {
            cerr << "The destination cell is found\n";
            trace_path();

            return true;
        }

        // Remove this vertex from the open set
        open_set.erase(open_set.begin());
        open_list[current.x][current.y] = 0;

        // Add this vertex to the closed list
        closed_list[current.x][current.y] = 1;

        cerr << "Add smallest f(" << current.x << "," << current.y << ") = "
        << cell_details[current.x][current.y].f_score << " to closed list" << endl;
        print_open_list();
        print_closed_list();

        /*
        Generating all the 4 successor of this cell

                   N
                   |
            W----cell----E
                  |
                  S

        cell-->Popped cell (current_x, current_y)
        N --> North	 (current_x, current_y+1)
        S --> South	 (current_x, current_y-1)
        W --> West	 (current_x-1, current_y)
        E --> East	 (current_x+1, current_y)
         */

        //----------- 1st Successor (North) ------------
        auto north = Coordinate{current.x, current.y + 1};
        // If the successor has not been evaluated and is passable
        if (closed_list[north.x][north.y]==0 && is_passable(north) && is_valid(north))
        {
            // cost of the cheapest path from start to n currently known
            int g_new = cell_details[current.x][current.y].g_score + 1;
            // h_score(n) estimates the cost to reach goal from node n
            int f_new = g_new + calculate_h(north);

            cerr << "north f_new: " << f_new << " ";
            cerr << "north f_current: " << cell_details[north.x][north.y].f_score << endl;
            if (f_new < cell_details[north.x][north.y].f_score) // new path is better
            {
                // Update the details of this cell
                cell_details[north.x][north.y] = Cell{g_new, f_new, current};
                // If it isn’t on the open list, add it to the open list.
                if(open_list[north.x][north.y]==0)
                {
                    open_list[north.x][north.y] = 1;
                    open_set.insert(OpenNode{f_new, north});

                    cerr << "Add north node (" << north.x << ", " << north.y << ") to open list" << endl;
                    print_open_list();
                    print_closed_list();
                }
            }
        }

        //----------- 2nd Successor (South) ------------
        auto south = Coordinate{current.x, current.y - 1};
        // Only process this cell if it is valid
        if (is_valid(south))
        {
            // If the successor has not been evaluated and is passable
            if (closed_list[south.x][south.y]==0 && is_passable(south))
            {
                int g_new = cell_details[current.x][current.y].g_score + 1;
                int f_new = g_new + calculate_h(south);

                cerr << "south f_new: " << f_new << " ";
                cerr << "south f_current: " << cell_details[south.x][south.y].f_score << endl;
                // if new path is better
                if (f_new < cell_details[south.x][south.y].f_score)
                {
                    // Update the details of this cell
                    cell_details[south.x][south.y] = Cell{g_new, f_new, current};
                    // If it isn’t on the open list, add it to the open list.
                    if(open_list[south.x][south.y]==0)
                    {
                        open_list[south.x][south.y] = 1;
                        open_set.insert(OpenNode{f_new, south});

                        cerr << "Add south node (" << south.x << ", " << south.y << ") to open list" << endl;
                        print_open_list();
                        print_closed_list();
                    }
                }
            }
        }

        //----------- 3rd Successor (West) ------------
        auto west = Coordinate{current.x - 1, current.y};
        // Only process this cell if this is a valid one
        if (is_valid(west))
        {
            // If the successor has not been evaluated and is passable
            if (closed_list[west.x][west.y]==0 && is_passable(west))
            {
                int g_new = cell_details[current.x][current.y].g_score + 1;
                int f_new = g_new + calculate_h(west);

                cerr << "west f_new: " << f_new << " ";
                cerr << "west f_current: " << cell_details[west.x][west.y].f_score << endl;
                // if new path is better
                if (f_new < cell_details[west.x][west.y].f_score)
                {
                    // Update the details of this cell
                    cell_details[west.x][west.y] = Cell{g_new, f_new, current};
                    // If it isn’t on the open list, add it to the open list.
                    if(open_list[west.x][west.y]==0)
                    {
                        open_list[west.x][west.y] = 1;
                        open_set.insert(OpenNode{f_new, west});

                        cerr << "Add west node (" << west.x << ", " << west.y << ") to open list" << endl;
                        print_open_list();
                        print_closed_list();
                    }
                }
            }
        }

        //----------- 4th Successor (East) ------------
        auto east = Coordinate{current.x + 1, current.y};
        // Only process this cell if this is a valid one
        if (is_valid(east))
        {
            // If the successor has not been evaluated and is passable
            if (closed_list[east.x][east.y]==0 && is_passable(east))
            {
                int g_new = cell_details[current.x][current.y].g_score + 1;
                int f_new = g_new + calculate_h(east);

                cerr << "east f_new: " << f_new << " ";
                cerr << "east f_current: " << cell_details[east.x][east.y].f_score << endl;
                // if new path is better
                if (f_new < cell_details[east.x][east.y].f_score)
                {
                    // Update the details of this cell
                    cell_details[east.x][east.y] = Cell{g_new, f_new, current};
                    // If it isn’t on the open list, add it to the open list.
                    if(open_list[east.x][east.y]==0)
                    {
                        open_list[east.x][east.y] = 1;
                        open_set.insert(OpenNode{f_new, east});

                        cerr << "Add east node (" << east.x << ", " << east.y << ") to open list" << endl;
                        print_open_list();
                        print_closed_list();
                    }
                }
            }
        }

        iter++;
    }

    // When the destination cell is not found and the open list is empty,
    // then we conclude that we failed to reach the destination cell.
    // This may happen when there is no way to destination cell (due to blockages)
    cerr << "Failed to find the Destination cell\n";
    return false;
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


void AStar::print_open_list()
{
    cerr << "open list: ";
    for(int i=0;i<num_rows;i++)
    {
        for(int j=0;j<num_columns;j++)
        {
            if(open_list[i][j] == 1)
            {
                cerr <<"f(" << i << ", " << j << ") = " << cell_details[i][j].f_score << " ";
            }
        }
    }
    cerr << endl;
}

void AStar::print_closed_list()
{
    cerr << "closed list: ";
    for(int i=0;i<num_rows;i++)
    {
        for(int j=0;j<num_columns;j++)
        {
            if(closed_list[i][j] == 1)
            {
                cerr <<"f(" << i << ", " << j << ") = " << cell_details[i][j].f_score << " ";
            }
        }
    }
    cerr << endl;
}

