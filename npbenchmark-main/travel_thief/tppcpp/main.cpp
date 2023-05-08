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
        line = regex_replace(line, regex("bounded strongly corr"), "bounded-strongly-corr");
        // cout << line << endl;

        // Skip comments and empty lines
        if(line.empty() || line[0] == '#')
        {
            continue;
        }

        // Parse the line
        string word_1, word_2, word_3, word_4;
        istringstream iss(line);
        iss >> word_1;

        if(word_1 == "PROBLEM")
        {
            iss >> word_2 >> word_3;
            cout << "PROBLEM NAME: " << word_3 << endl;
        }
        else if(word_1 == "KNAPSACK")
        {
            iss >> word_2 >> word_3 >> word_4;
            cout << "KNAPSACK DATA TYPE: " << word_4 << endl;
        }
        else if(word_1 == "DIMENSION:")
        {
            iss >> word_2;
            int dimension = stoi(word_2);
            cout << "DIMENSION: " << dimension << endl;
        }
        else if(word_1 == "NUMBER")
        {
            iss >> word_2 >> word_3 >> word_4;
            int num_items = stoi(word_4);
            cout << "NUMBER OF ITEMS: " << num_items << endl;
        }
        else if(word_1 == "CAPACITY")
        {
            iss >> word_2 >> word_3 >> word_4;
            int capacity = stoi(word_4);
            cout << "CAPACITY OF KNAPSACK: " << capacity << endl;
        }
        else if(word_1 == "MIN")
        {
            iss >> word_2 >> word_3;
            float min_speed = stof(word_3);
            cout << "MIN SPEED: " << min_speed << endl;
        }
        else if(word_1 == "MAX")
        {
            iss >> word_2 >> word_3;
            float max_speed = stof(word_3);
            cout << "MAX SPEED: " << max_speed << endl;
        }
        else if(word_1 == "RENTING")
        {
            iss >> word_2 >> word_3;
            float renting_ratio = stof(word_3);
            cout << "RENTING RATIO: " << renting_ratio << endl;
        }
        else if(word_1 == "EDGE_WEIGHT_TYPE:")
        {
            iss >> word_2;
            cout << "Edge weight type: " << word_2 << endl;
        }
        else if(word_1 == "NODE_COORD_SECTION")
        {
            iss >> word_2 >> word_3 >> word_4;
            // Read the node coordinates section
            cout << "Node coordinates:" << word_2 << word_3 << word_4 << endl;

            int index;
            float x, y;
            while (file >> index >> x >> y)
            {
                cout << index << " (" << x << ", " << y << ")" << endl;
            }
        }
        else if(word_1 == "ITEMS")
        {
            iss >> word_2;
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
