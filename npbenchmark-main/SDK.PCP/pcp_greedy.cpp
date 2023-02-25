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
                       :dbs_covered(input_num_vertex)
                       ,dbs_uncovered(input_num_vertex)
                       ,dbs_equal_delta{}
                       ,equal_delta{}
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

    // assign value to center_cover_vertex and vertex_reach_center;
    center_cover_vertex.resize(input_num_vertex, boost::dynamic_bitset<>(input_num_vertex));
    vertex_reach_center.resize(input_num_vertex, boost::dynamic_bitset<>(input_num_vertex));
    for(int i=0;i<input_coverages.size();i++) // i is center name;
    {
        for(int j=0;j<input_coverages[i].size();j++) // j is vertex name;
        {
            int index = input_coverages[i][j];
            center_cover_vertex[i].set(index, true);
            vertex_reach_center[index].set(i, true);
        }
        // cerr << "center_cover_vertex[" << i << "] " << center_cover_vertex[i] << endl;
    }
    // cerr << endl;

    // debug the assignment of center_cover_vertex and vertex_reach_center;
    // cerr << "center_cover_vertex[0]: " << center_cover_vertex[0] << endl;
    // cerr << "center_cover_vertex[99]: " << center_cover_vertex[99] << endl;
    // cerr << "vertex_reach_center[0]: " << vertex_reach_center[0] << endl;

    nodes_with_drops.resize(input_nodesWithDrops.size());
    for(int i=0;i<input_nodesWithDrops.size();i++)
    {
        for(int j=0;j<input_nodesWithDrops[i].size();j++)
        {
            nodes_with_drops[i].push_back(input_nodesWithDrops[i][j]);
        }
    }

    dbs_covered.reset(); // set dbs_covered all 0;
    dbs_uncovered.set(); // set dbs_uncovered all 1;

    iter = 0;
}

PCP_Greedy::~PCP_Greedy()
= default;

void PCP_Greedy::greedy_search(vector<vector<int>> &input_coverages)
{
    if(nodes_with_drops.empty())
    {
        int equal_count = 0;
        int dbs_equal_count = 0;

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

        while(selected.size()<num_center && covered.size()!=num_vertex && iter<1) // do one iteration;
        {
            cerr << "iteration: " << iter << endl;
            int max_overlap_size = 0;
            int max_overlap_index = 0;

            unsigned long long dbs_max_overlap_size = 0;
            int dbs_max_overlap_index = 0;

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

            // cerr << "dbs_uncovered" << dbs_uncovered << endl;
            for(int j=0;j<num_vertex;j++) // consider only one set;
            {
                boost::dynamic_bitset<> this_intersection = dbs_uncovered & center_cover_vertex[j];
                unsigned long long this_intersection_size = this_intersection.count();

                if(this_intersection_size > dbs_max_overlap_size)
                {
                    dbs_max_overlap_size = this_intersection_size;
                    dbs_max_overlap_index = j;

                    dbs_equal_count = 0;
                    dbs_equal_delta[dbs_equal_count] = j; // j is index of center;
                    dbs_equal_count++;
                }
                else if(this_intersection_size == dbs_max_overlap_size)
                {
                    dbs_equal_delta[dbs_equal_count] = j; // j is index of center;
                    dbs_equal_count++;
                }
            }

            cerr << "dbs equal count: " << dbs_equal_count << endl;

            cerr << "dbs_max_overlap_index: " << dbs_max_overlap_index <<endl;
            int dbs_rand_select = rand_select; // 相等tabu_delta随机选择
            cerr << "dbs random select: " << dbs_rand_select <<endl;
            cerr << "dbs random select index: " << dbs_equal_delta[dbs_rand_select] <<endl;

            selected.push_back(equal_delta[rand_select]);
            dbs_solution.push_back(dbs_equal_delta[dbs_rand_select]);

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

            dbs_covered = dbs_covered | center_cover_vertex[dbs_equal_delta[dbs_rand_select]];
            cerr << "DBS Cover after union size (" << dbs_covered.count() << "): " << endl;
            cerr << dbs_covered << endl;

            vector<int> difference_result;
            set_difference(universe.begin(),universe.end(),
                           covered.begin(),covered.end(),
                           back_inserter(difference_result));

            cerr << "Uncover after union are: ";
            for (int & it : difference_result)
                cerr << it << " ";
            cerr << endl;

            uncovered.assign(difference_result.begin(), difference_result.end());

            dbs_uncovered = ~dbs_covered;

            cerr << "DBS Uncover after union are: " << endl;
            cerr << dbs_uncovered << endl;

            iter++;
        }

        cerr << "Center selected are: ";
        for (int & it : selected)
            cerr << it << " ";
        cerr << endl;

        cerr << "DBS Center selected are: ";
        for (int & it : dbs_solution)
            cerr << it << " ";
        cerr << endl;
    }


}




