//
// Created by take_ on 2022/12/10.
//

# include "pcp_local_search.h"

# include <algorithm>
# include <iostream>
# include <random>

using namespace std;

mt19937 pseudoRandNumGen;
void initRand(int seed) { pseudoRandNumGen = mt19937(seed); }

PCP_Local_Search::PCP_Local_Search(int input_nodeNum, int input_centerNum, vector<vector<int>> &input_coverages, vector<vector<int>> &input_nodesWithDrops, int input_seed)
{
    seed = input_seed;
    initRand(seed); // initialize random generator;

    num_node = input_nodeNum;
    num_center = input_centerNum;
    center_coverages.resize(input_coverages.size());
    nodes_with_drops.resize(input_nodesWithDrops.size());

    for(int i=0;i<input_coverages.size();i++)
    {
        for(int j=0;j<input_coverages[i].size();j++)
        {
            center_coverages[i].push_back(input_coverages[i][j]);
        }
    }

    for(int i=0;i<input_nodesWithDrops.size();i++)
    {
        for(int j=0;j<input_nodesWithDrops[i].size();j++)
        {
            nodes_with_drops[i].push_back(input_nodesWithDrops[i][j]);
        }
    }

    try
    {
        // allocate memory and initial value for solution (dim, num_center);
        solution = new unsigned int[num_center];
        for (int i = 0; i < num_center; i++)
            solution[i] = pseudoRandNumGen() % num_node;

    }
    catch (const bad_alloc& e)
    {
        cerr << "初始化内存分配失败:" << endl;
    }

    for(int i=0;i<input_nodeNum;i++)
    {
        universe.push_back(i);
    }

    for(int i=0;i<input_nodeNum;i++)
    {
        uncovered.push_back(i);
    }
}

PCP_Local_Search::~PCP_Local_Search()
{

}

void PCP_Local_Search::swap_center(int center_out, int center_in)
{

}

void PCP_Local_Search::local_search()
{
    if(nodes_with_drops.size()==0)
    {
        cerr << "Initial solution: ";
        for (int i = 0; i < num_center; i++)
            cerr << solution[i] <<" ";
        cerr << endl;

    }

    vector<int> test_member_merge;
    set_union(center_coverages[59].begin(),center_coverages[59].end(),
              center_coverages[12].begin(),center_coverages[12].end(),
              back_inserter(test_member_merge));
    cerr << "Test merge size (" <<test_member_merge.size()<<"): ";
    for (int & it : test_member_merge)
        cerr << it << " ";
    cerr << endl;


}



















