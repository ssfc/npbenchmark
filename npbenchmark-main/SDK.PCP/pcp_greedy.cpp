//
// Created by take_ on 2022/12/10.
//
# include "pcp_greedy.h"
# include <algorithm>
# include <iostream>
# include <random>

using namespace std;

mt19937 pseudoRandNumGen;
void initRand(int seed) { pseudoRandNumGen = mt19937(seed); }

PCP_Greedy::PCP_Greedy(int input_num_vertex, int input_num_center,
                       vector<vector<int>> &input_coverages, vector<vector<int>> &input_nodesWithDrops, int seed)
                       :equal_delta{}
{
    initRand(seed); // initialize random generator;

    num_vertex = input_num_vertex;
    num_center = input_num_center;

    center_coverages.resize(input_num_vertex);
    for(int i=0;i<input_coverages.size();i++)
    {
        center_coverages[i].resize(input_num_vertex);
        for(int j=0;j<input_coverages[i].size();j++)
        {
            int index = input_coverages[i][j];
            center_coverages[i][index] = 1;
        }
    }

    nodes_with_drops.resize(input_nodesWithDrops.size());
    for(int i=0;i<input_nodesWithDrops.size();i++)
    {
        for(int j=0;j<input_nodesWithDrops[i].size();j++)
        {
            nodes_with_drops[i].push_back(input_nodesWithDrops[i][j]);
        }
    }

    iter = 0;
}

PCP_Greedy::~PCP_Greedy()
= default;

void PCP_Greedy::greedy_search(vector<vector<int>> &input_coverages)
{
    if(nodes_with_drops.empty())
    {
        int equal_count = 0;

        for(int i=0;i<num_vertex;i++)
        {
            universe.push_back(i);
        }

        for(int i=0;i<num_vertex;i++)
        {
            uncovered.push_back(i);
        }

        /* // test whether union two sets is correct or not;
        vector<int> test_merge;
        set_union(input.coverages[59].begin(),input.coverages[59].end(),
                         input.coverages[12].begin(),input.coverages[12].end(),
                         back_inserter(test_merge));
        cerr << "Test merge size (" <<test_merge.size()<<"): ";
        for (int & it : test_merge)
            cerr << it << " ";
        cerr << endl;
        */

        while(selected.size()<num_center && covered.size()!=num_vertex) // do one iteration;
        {
            cerr << "iteration: " << iter << endl;
            int max_overlap_size = 0;
            int max_overlap_index = 0;
            for(int j=0;j<num_vertex;j++) // consider only one set;
            {
                vector<int> this_intersection;
                set_intersection(uncovered.begin(),uncovered.end(),
                                 input_coverages[j].begin(),input_coverages[j].end(),
                                 back_inserter(this_intersection));

                if(this_intersection.size() > max_overlap_size)
                {
                    max_overlap_size = this_intersection.size();
                    max_overlap_index = j;

                    equal_count = 0;
                    equal_delta[equal_count] = j; // j is index of center;
                    equal_count++;
                }
                else if(this_intersection.size() == max_overlap_size)
                {
                    equal_delta[equal_count] = j; // j is index of center;
                    equal_count++;
                }
            }

            cerr << "equal count: " << equal_count << endl;

            cerr << "max_overlap_index: " << max_overlap_index <<endl;
            int rand_select = pseudoRandNumGen() % equal_count; // 相等tabu_delta随机选择
            cerr << "random select: " << rand_select <<endl;
            cerr << "random select index: " << equal_delta[rand_select] <<endl;

            selected.push_back(max_overlap_index);
            vector<int> union_result;
            set_union(covered.begin(),covered.end(),
                      input_coverages[max_overlap_index].begin(),input_coverages[max_overlap_index].end(),
                      back_inserter(union_result));

            cerr << "The results of union "<<max_overlap_index<<" are: ";
            for (int & it : union_result)
                cerr << it << " ";
            cerr << endl;

            covered.assign(union_result.begin(), union_result.end());

            cerr << "Cover after union size (" << covered.size() << "): ";
            for (int & it : union_result)
                cerr << it << " ";
            cerr << endl;

            vector<int> difference_result;
            set_difference(universe.begin(),universe.end(),
                           covered.begin(),covered.end(),
                           back_inserter(difference_result));

            cerr << "Uncover after union are: ";
            for (int & it : difference_result)
                cerr << it << " ";
            cerr << endl;

            uncovered.assign(difference_result.begin(), difference_result.end());

        }

        cerr << "Point selected are: ";
        for (int & it : selected)
            cerr << it << " ";
        cerr << endl;

        iter++;
    }


}




