//
// Created by take_ on 2022/12/10.
//

#ifndef SDK_PCP_PCP_LOCAL_SEARCH_H
#define SDK_PCP_PCP_LOCAL_SEARCH_H

# include <vector>

using namespace std;

class PCP_Local_Search
{
private: // member variables.
    int num_node;
    int num_center;
    vector<vector<int>> center_coverages;
    vector<vector<int>> nodes_with_drops;

    unsigned int* solution; // index of selected center; dimension, num_center;
    vector<int> universe; // all points;
    vector<int> covered; // points already covered by set;
    vector<int> uncovered; // points has not been covered by set;

public: // member functions.
    PCP_Local_Search(int input_nodeNum, int input_centerNum, vector<vector<int>>& input_coverages, vector<vector<int>> &input_nodesWithDrops, int seed);
    ~PCP_Local_Search();

    void swap_center(int center_out, int center_in);
    void local_search(int seed);
};










#endif //SDK_PCP_PCP_LOCAL_SEARCH_H
