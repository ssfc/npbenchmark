#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>

using namespace std;

int main()
{
    string filename = "../data/a280_n279_bounded-strongly-corr_01.ttp";
    ifstream file(filename);

    if(!file.is_open())
    {
        cerr << "Failed to open file " << filename << endl;
        return 1;
    }

    string line;
    while(getline(file, line))
    {
        // Remove leading and trailing whitespace
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line = regex_replace(line, regex(" \\t "), "\t");
        line = regex_replace(line, regex("PROBLEM NAME"), "problem_name");
        line = regex_replace(line, regex("KNAPSACK DATA TYPE"), "knapsack_data_type");
        line = regex_replace(line, regex("bounded strongly corr"), "bounded-strongly-corr");
        line = regex_replace(line, regex("NUMBER OF ITEMS"), "number_of_items");
        line = regex_replace(line, regex("CAPACITY OF KNAPSACK"), "capacity_of_knapsack");
        line = regex_replace(line, regex("MIN SPEED"), "min_speed");
        line = regex_replace(line, regex("MAX SPEED"), "max_speed");
        line = regex_replace(line, regex("RENTING RATIO"), "renting_ratio");
        line = regex_replace(line, regex("(INDEX, X, Y):"), "(index_X_Y):");
        // cout << line << endl;

        // Skip comments and empty lines
        if(line.empty() || line[0] == '#')
        {
            continue;
        }

        // Parse the line
        string key, value;
        istringstream iss(line);
        iss >> key;

        if(key == "problem_name:")
        {
            iss >> value;
            cout << "Problem name: " << value << endl;
        }
        else if(key == "knapsack_data_type:")
        {
            iss >> value;
            cout << "Knapsack data type: " << value << endl;
        }
        else if(key == "DIMENSION:")
        {
            iss >> value;
            int dimension = stoi(value);
            cout << "Dimension: " << dimension << endl;
        }
        else if(key == "number_of_items:")
        {
            iss >> value;
            int num_items = stoi(value);
            cout << "Number of items: " << num_items << endl;
        }
        else if(key == "capacity_of_knapsack:")
        {
            iss >> value;
            int capacity = stoi(value);
            cout << "Capacity of knapsack: " << capacity << endl;
        }
        else if(key == "min_speed:")
        {
            iss >> value;
            float min_speed = stof(value);
            cout << "Min speed: " << min_speed << endl;
        }
        else if(key == "max_speed:")
        {
            iss >> value;
            float max_speed = stof(value);
            cout << "Max speed: " << max_speed << endl;
        }
        else if(key == "renting_ratio:")
        {
            iss >> value;
            float renting_ratio = stof(value);
            cout << "Renting ratio: " << renting_ratio << endl;
        }
        else if(key == "EDGE_WEIGHT_TYPE:")
        {
            iss >> value;
            cout << "Edge weight type: " << value << endl;
        }
        else if(key == "NODE_COORD_SECTION")
        {
            iss >> value;
            // Read the node coordinates section
            cout << "Node coordinates:" << value << endl;

            int index;
            float x, y;
            while (file >> index >> x >> y)
            {
                cout << index << " (" << x << ", " << y << ")" << endl;
            }
        }
        else if(key == "ITEMS")
        {
            iss >> value;
            // Read the node coordinates section
            cout << "Items section:" << endl;

            // INDEX, PROFIT, WEIGHT, ASSIGNED NODE NUMBER
            int index;
            float profit, weight, assigned_node_number;
            while (file >> index >> profit >> weight >> assigned_node_number)
            {
                cout << index << " (" << profit << ", " << weight << ") " << assigned_node_number << endl;
            }
        }
    }

    file.close();
    return 0;
}
