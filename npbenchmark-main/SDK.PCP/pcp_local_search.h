//
// Created by take_ on 2022/12/10.
//

#ifndef SDK_PCP_PCP_LOCAL_SEARCH_H
#define SDK_PCP_PCP_LOCAL_SEARCH_H

# include <vector>

using namespace std;

class PCP_Local_Search
{
public:
    PCP_Local_Search(int input_nodeNum, int input_centerNum, vector<vector<int>>& input_coverages, vector<vector<int>> &input_nodesWithDrops, int seed);
    ~PCP_Local_Search();

    void local_search(int input_nodeNum, int input_centerNum, vector<vector<int>> &input_coverages, vector<vector<int>> &input_nodesWithDrops, int seed);
};










#endif //SDK_PCP_PCP_LOCAL_SEARCH_H
