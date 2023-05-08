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
        string first, second;
        istringstream iss(line);
        iss >> first;

        if(first == "problem_name:")
        {
            iss >> second;
            cout << "Problem name: " << second << endl;
        }
        else if(first == "knapsack_data_type:")
        {
            iss >> second;
            cout << "Knapsack data type: " << second << endl;
        }
        else if(first == "DIMENSION:")
        {
            iss >> second;
            int dimension = stoi(second);
            cout << "Dimension: " << dimension << endl;
        }
        else if(first == "number_of_items:")
        {
            iss >> second;
            int num_items = stoi(second);
            cout << "Number of items: " << num_items << endl;
        }
        else if(first == "capacity_of_knapsack:")
        {
            iss >> second;
            int capacity = stoi(second);
            cout << "Capacity of knapsack: " << capacity << endl;
        }
        else if(first == "min_speed:")
        {
            iss >> second;
            float min_speed = stof(second);
            cout << "Min speed: " << min_speed << endl;
        }
        else if(first == "max_speed:")
        {
            iss >> second;
            float max_speed = stof(second);
            cout << "Max speed: " << max_speed << endl;
        }
        else if(first == "renting_ratio:")
        {
            iss >> second;
            float renting_ratio = stof(second);
            cout << "Renting ratio: " << renting_ratio << endl;
        }
        else if(first == "EDGE_WEIGHT_TYPE:")
        {
            iss >> second;
            cout << "Edge weight type: " << second << endl;
        }
        else if(first == "NODE_COORD_SECTION")
        {
            iss >> second;
            // Read the node coordinates section
            cout << "Node coordinates:" << second << endl;

            int index;
            float x, y;
            while (file >> index >> x >> y)
            {
                cout << index << " (" << x << ", " << y << ")" << endl;
            }
        }
        else if(first == "ITEMS")
        {
            iss >> second;
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
