//
// Created by take_ on 2022/12/13.
//
# include "pcp_vector.h"

# include <algorithm>
# include <climits>
# include <iostream>
# include <random>
# include <vector>

using namespace std;

mt19937 pseudoRandNumGen;
void initRand(int seed) { pseudoRandNumGen = mt19937(seed); }

PCP_Vector::PCP_Vector(int input_nodeNum, int input_centerNum, vector<vector<int>> &input_coverages, vector<vector<int>> &input_nodesWithDrops, int input_seed)
{
    seed = input_seed;
    initRand(seed); // initialize random generator;

    num_node = input_nodeNum;
    num_center = input_centerNum;
    center_coverages.resize(input_nodeNum);
    nodes_with_drops.resize(input_nodesWithDrops.size());

    for(int i=0;i<input_coverages.size();i++)
    {
        center_coverages[i].resize(input_nodeNum);
        for(int j=0;j<input_coverages[i].size();j++)
        {
            int index = input_coverages[i][j];
            center_coverages[i][index] = 1;
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

    covered.resize(input_nodeNum);
    for(int i=0;i<input_nodeNum;i++)
    {
        covered[i] = 0;
    }
}

PCP_Vector::~PCP_Vector()
{

}

void PCP_Vector::swap_center(int center_out, int center_in)
{
    for(int i=0;i<covered.size();i++)
    {
        covered[i] -= center_coverages[center_out][i];
    }

    for(int i=0;i<covered.size();i++)
    {
        covered[i] += center_coverages[center_in][i];
    }
}

void PCP_Vector::find_move()
{
    vector<int> solution_vector; // make solution as 001010, in which solution member is 1's index;
    vector<int> set_selected;
    vector<int> set_unselected;

    solution_vector.resize(num_node);
    for(int i=0;i<num_center;i++)
    {
        solution_vector[solution[i]] = 1;
    }

    for(int i=0;i<solution_vector.size();i++)
    {
        if(solution_vector[i]==0)
        {
            set_unselected.push_back(i);
        }
        else
        {
            set_selected.push_back(i);
        }
    }

    unsigned int min_uncovered_size = INT_MAX;
    int center_out;
    int center_in;
    for(int i : set_selected)
    {
        for(int j : set_unselected)
        {
            vector<int> temp;
            temp.resize(covered.size());
            temp.assign(covered.begin(), covered.end());

            for(int k=0;k<covered.size();k++)
            {
                temp[k] -= center_coverages[i][k];
            }

            for(int k=0;k<covered.size();k++)
            {
                temp[k] += center_coverages[j][k];
            }

            if(count(temp.begin(), temp.end(), 0) < min_uncovered_size) // the less the better;
            {
                min_uncovered_size = count(temp.begin(), temp.end(), 0);
                cerr << "current min_uncovered_size: " << min_uncovered_size << endl;
                center_out = i;
                center_in = j;
            }
        }
    }
    cerr << "center_out: " << center_out << endl;
    cerr << "center_in: " << center_in << endl;
    for(int i=0;i<num_center;i++)
    {
        if(solution[i] == center_out)
        {
            solution[i] = center_in;
        }
    }
    swap_center(center_out, center_in);
    cerr << "current solution: ";
    for (int i = 0; i < num_center; i++)
        cerr << solution[i] <<" ";
    cerr << endl;
    cerr << "number of uncovered after first find move: " << count(covered.begin(), covered.end(), 0) << endl;
}

void PCP_Vector::local_search()
{
    if(nodes_with_drops.empty())
    {
        cerr << "Initial solution: ";
        for (int i = 0; i < num_center; i++)
            cerr << solution[i] <<" ";
        cerr << endl;

        for(int j=0;j<num_center;j++)
        {
            for(int i=0;i<covered.size();i++)
            {
                covered[i] += center_coverages[j][i];
            }
        }

        unsigned int num_uncovered = count(covered.begin(), covered.end(), 0);
        cerr << "number of uncovered in initial solution: " << num_uncovered << endl;

        find_move();

    }


}

