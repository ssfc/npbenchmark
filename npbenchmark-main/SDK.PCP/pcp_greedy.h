//
// Created by take_ on 2022/12/10.
//

#ifndef SDK_PCP_PCP_GREEDY_H
#define SDK_PCP_PCP_GREEDY_H

# include <vector>
#include <boost/dynamic_bitset.hpp>

using namespace std;

class PCP_Greedy
{
private:
    int num_vertex;
    int num_center;
    vector<vector<int>> center_coverages;
    // Dimension: num_vertex * num_vertex
    // Element: true/false;
    // Meaning: if center i covers vertex j, center_cover_vertex[i][j] = true; else = false;
    vector<boost::dynamic_bitset<>> center_cover_vertex;

    // Dimension: num_vertex * num_vertex
    // Element: true/false;
    // Meaning: if vertex i can reach center j, vertex_reach_center[i][j] = true; else = false;
    vector<boost::dynamic_bitset<>> vertex_reach_center;
    vector<vector<int>> nodes_with_drops;

    vector<int> dbs_solution; // centers selected;

    boost::dynamic_bitset<> dbs_covered;
    boost::dynamic_bitset<> dbs_uncovered;

    int dbs_equal_delta[2000]; //非禁忌相同delta值
    long long int iter;
public:
    PCP_Greedy(int input_num_vertex, int input_num_center, vector<vector<int>>& input_coverages, vector<vector<int>> &input_nodesWithDrops, int seed);
    ~PCP_Greedy();

    void greedy_search(vector<vector<int>> &input_coverages);
};

#endif //SDK_PCP_PCP_GREEDY_H
