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
    int num_vertex;
    int num_center;
    vector<vector<int>> center_coverages;
    vector<vector<int>> nodes_with_drops;

    vector<int> selected; // centers selected;

    vector<int> universe; // all points;
    vector<int> covered; // points already covered by set;
    vector<int> uncovered; // points has not been covered by set;

    int equal_delta[2000]; //非禁忌相同delta值
    long long int iter;
public:
    PCP_Greedy(int input_num_vertex, int input_num_center, vector<vector<int>>& input_coverages, vector<vector<int>> &input_nodesWithDrops, int seed);
    ~PCP_Greedy();

    void greedy_search(vector<vector<int>> &input_coverages);
};

#endif //SDK_PCP_PCP_GREEDY_H
