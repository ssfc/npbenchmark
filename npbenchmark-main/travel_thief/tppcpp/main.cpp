#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>

using namespace std;

int main() {
    string filename = "../data/a280_n279_bounded-strongly-corr_01.ttp";
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Failed to open file " << filename << endl;
        return 1;
    }

    string line;
    while (getline(file, line)) {
        // Remove leading and trailing whitespace
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line = regex_replace(line, regex(" \\t "), "\t");
        line = regex_replace(line, regex("PROBLEM NAME"), "problem_name");
        line = regex_replace(line, regex("KNAPSACK DATA TYPE"), "knapsack_data_type");
        line = regex_replace(line, regex("bounded strongly corr"), "bounded-strongly-corr");
        line = regex_replace(line, regex("NUMBER OF ITEMS"), "number_of_items");
        // cout << line << endl;

        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Parse the line
        string key, value;
        istringstream iss(line);
        iss >> key >> value;

        if (key == "problem_name:") {
            cout << "Problem name: " << value << endl;
        } else if (key == "knapsack_data_type:") {
            cout << "Knapsack data type: " << value << endl;
        } else if (key == "DIMENSION:") {
            int dimension = stoi(value);
            cout << "Dimension: " << dimension << endl;
        } else if (key == "number_of_items:") {
            int num_items = stoi(value);
            cout << "Number of items: " << num_items << endl;
        } else if (key == "CAPACITY") {
            int capacity = stoi(value);
            cout << "Capacity of knapsack: " << capacity << endl;
        } else if (key == "MIN") {
            float min_speed = stof(value);
            cout << "Min speed: " << min_speed << endl;
        } else if (key == "MAX") {
            float max_speed = stof(value);
            cout << "Max speed: " << max_speed << endl;
        } else if (key == "RENTING") {
            float renting_ratio = stof(value);
            cout << "Renting ratio: " << renting_ratio << endl;
        } else if (key == "EDGE_WEIGHT_TYPE:") {
            cout << "Edge weight type: " << value << endl;
        } else if (key == "NODE_COORD_SECTION") {
            // Read the node coordinates section
            cout << "Node coordinates:" << endl;

            int index;
            float x, y;
            while (file >> index >> x >> y) {
                cout << index << " (" << x << ", " << y << ")" << endl;
            }
        }
    }

    file.close();
    return 0;
}
