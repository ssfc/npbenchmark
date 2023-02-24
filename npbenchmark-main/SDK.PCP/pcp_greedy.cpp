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

PCP_Greedy::PCP_Greedy(int input_center_num, int input_num_color, vector<vector<int>> &input_coverages, vector<vector<int>> &input_nodesWithDrops, int seed)
{

}

PCP_Greedy::~PCP_Greedy()
= default;

void PCP_Greedy::greedy_search(int input_nodeNum, int input_centerNum, vector<vector<int>> &input_coverages, vector<vector<int>> &input_nodesWithDrops, int seed)
{
    initRand(seed); // initialize random generator;

    if(input_nodesWithDrops.empty())
    {
        int equal_count = 0;

        for(int i=0;i<input_nodeNum;i++)
        {
            universe.push_back(i);
        }

        for(int i=0;i<input_nodeNum;i++)
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

        for(int i=0; selected.size()<input_centerNum && covered.size()!=input_nodeNum; i++) // do one iteration;
        {
            cerr<<"iteration: "<<i<<endl;
            int max_overlap_size = 0;
            int max_overlap_index = 0;
            for(int j=0;j<input_nodeNum;j++) // consider only one set;
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

    }


}




