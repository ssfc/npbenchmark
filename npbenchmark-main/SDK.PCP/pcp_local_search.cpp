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

PCP_Local_Search::PCP_Local_Search(int input_nodeNum, int input_centerNum, vector<vector<int>> &input_coverages, vector<vector<int>> &input_nodesWithDrops, int seed)
{
    initRand(seed); // initialize random generator;

    num_node = input_nodeNum;
    num_center = input_centerNum;

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
}

PCP_Local_Search::~PCP_Local_Search()
{

}

void PCP_Local_Search::local_search(vector<vector<int>> &input_coverages, vector<vector<int>> &input_nodesWithDrops, int seed)
{
    if(input_nodesWithDrops.size()==0)
    {
        cerr << "Initial solution: ";
        for (int i = 0; i < num_center; i++)
            cerr << solution[i] <<" ";
        cerr << endl;

    }


}



















