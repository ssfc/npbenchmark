//
// Created by take_ on 2023/1/4.
//

#ifndef YUANFANG1_GCP_HEA_H
#define YUANFANG1_GCP_HEA_H

using namespace std;

class Hybrid_Evolution
{
private:

public:
    void dynamic_alloc();
    void insert_adj_list(int i, int j);
    int tabu_search(int *solution);



    // debug function: compute conflict of a solution
    int compute_conflict(int *solution);
};


#endif //YUANFANG1_GCP_HEA_H
