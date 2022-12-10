//
// Created by take_ on 2022/12/10.
//

#ifndef SDK_PCP_PCP_GREEDY_H
#define SDK_PCP_PCP_GREEDY_H

# include <vector>

using namespace std;

class PCP_Greedy
{
public:
    PCP_Greedy(int input_center_num, int input_num_color, vector<vector<int>>& input_coverages, int seed);
    ~PCP_Greedy();

    void greedy_find(int input_center_num, int input_num_color, vector<vector<int>> &input_coverages, int seed);
};

#endif //SDK_PCP_PCP_GREEDY_H
