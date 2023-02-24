//
// Created by take_ on 2022/12/10.
//

#ifndef SDK_PCP_PCP_GREEDY_H
#define SDK_PCP_PCP_GREEDY_H

# include <vector>

using namespace std;

class PCP_Greedy
{
private:
    vector<int> selected; // centers selected;

    vector<int> universe; // all points;
    vector<int> covered; // points already covered by set;
    vector<int> uncovered; // points has not been covered by set;

    int equal_delta[2000]; //非禁忌相同delta值
public:
    PCP_Greedy(int input_nodeNum, int input_centerNum, vector<vector<int>>& input_coverages, vector<vector<int>> &input_nodesWithDrops, int seed);
    ~PCP_Greedy();

    void greedy_search(int input_nodeNum, int input_centerNum, vector<vector<int>> &input_coverages, vector<vector<int>> &input_nodesWithDrops, int seed);
};

#endif //SDK_PCP_PCP_GREEDY_H
