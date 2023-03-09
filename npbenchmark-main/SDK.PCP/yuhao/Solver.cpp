#include <functional>
#include <random>
#include <chrono>
#include <iostream>
#include <algorithm>

#include "../.h/PCenter.h"
#include "../.h/Solver.h"
#include "../.h/UCoverX.h"
#include "../.h/Cover.h"

using namespace std;

void Solver::init(int seed)
{ 
    this->e.seed(seed);
}

int Solver::rand(int max, int min = 0) 
{ 
    return uniform_int_distribution<int>(min, max - 1)(this->e);
}

void Solver::calculate(int*& output, PCenter& input, UCoverX& UX, function<bool()> isTimeout, int r = -1) 
{
    // algorithm 1 lies here.
    // clock_t start_time = clock();
    if(r != -1){
        if(UX.size() == 0)
            return;
        else
            reInit(input);
    }

    else
        Init(input, UX);

    int iter = 0, UXSize = UX.size(), lastUXSize = UXSize, minUXSize = UXSize, Size, node, cnode, v, l;

    if(UXSize == 0){
        copyX(output, input.X, input.centerNum);
        return;
    }

    for(v = 0;v < input.centerNum;v++){
        node = input.X[v];
        Size = input.sizes[node];
        for(l = 0;l < Size;l++){
            cnode = input.coverages[node][l];
            if(input.covers[cnode].size() == 1){
                input.Delta[node] += input.w[cnode];
            }
        }
    }

    for(v = 0;v < lastUXSize;v++){
        node = UX[v];
        Size = input.sizes[node];
        for(l = 0;l < Size;l++){
            input.Delta[input.coverages[node][l]] += input.w[node];
        }
    }

    // cerr << "\n###A new iteration begins as follow.###" << endl;

    // cerr << "start : UXSize = " << UXSize<< endl;

    for(;!isTimeout();iter++){

        std::pair<int, int> p = this->FindPair(input, UX, UXSize, iter);

        this->MakeMove(input, UX, p.first, p.second);

        UXSize = UX.size();

        if(UXSize < lastUXSize){
            if(UXSize < minUXSize){
                minUXSize = UXSize;
                if(UXSize == 0){
                    copyX(output, input.X, input.centerNum);
                    break;
                }
            }
        }

        else if(UXSize >= lastUXSize){
            for(v = 0;v < UXSize;v++){
                node = UX[v];
                Size = input.sizes[node];
                input.w[node]++;
                for(l = 0;l < Size;l++){
                    input.Delta[input.coverages[node][l]]++;
                }
            }
        }

        lastUXSize = UXSize;

        input.tabulist[p.first] = iter + TABUTT;
        input.tabulist[p.second] = iter + TABUTT;

    }

    // cerr << "end : UXSize = " << UXSize << endl;

    // cerr << "Current iter = " << iter << endl;

    // double duration = (double)(clock() - start_time) / CLOCKS_PER_SEC;

    // cerr << "Current iteration run time is : " <<  duration << "s" << endl;

    // cerr << "Current iterations pre second is : " << iter / duration << endl;

    // cerr << "###Current iteration ends.###" << endl;
}

void Solver::reducing(PCenter& input, UCoverX& UX, int& r) {
    int n = 0, dropsize = input.dropsSizes[r], node, nodep, *p = nullptr;
    Cover* c = nullptr;
    for (; n < dropsize; n++) {
        // input.sizes[input.nodesWithDrops[r][n]]--;
        node = input.nodesWithDrops[r][n];
        p = &input.sizes[node];
        (*p)--;
        nodep = input.coverages[node][*p];
        if(input.positions[node] >= 0){
            c = &input.covers[nodep];
            c->erase(node);
            if(c->size() == 0)
                UX.push_back(input, nodep);
        }
    }
}

void Solver::copyX(int*& des, int*& src, int& centerNumber)
{
    for(int i = 0;i < centerNumber;i++)
        des[i] = src[i];
}

void Solver::reInit(PCenter& input)
{
    for(int v = 0;v < input.nodeNum;v++){
        input.tabulist[v] = 0;
        input.w[v] = 1;
        input.Delta[v] = 0;
    }
}

void Solver::Init(PCenter& input, UCoverX& UX)
{
    int maxCover, currentCover, tempXcount, i, chosenNode , pNumber = input.Xindex;

    for(;pNumber < input.centerNum;pNumber++){

        maxCover = 0;

        for(i = 0;i < input.nodeNum;i++){
            if(this->Xposition(input, i) >= 0)
                continue;

            currentCover = this->InterUX(input, UX, i);
            if(currentCover > maxCover){
                maxCover = currentCover;
                chosenNode = i;
                tempXcount = 1;
            }
            else if(currentCover == maxCover && this->rand(++tempXcount) == 0){
                chosenNode = i;
            }
        }

        this->UpdateUX(input, UX, chosenNode, pNumber);

    }

}

int Solver::Uposition(PCenter& input, int& id)
{
    return -input.positions[id] - 2;
}

int Solver::Xposition(PCenter& input, int &id)
{
    return input.positions[id];
}

int Solver::InterUX(PCenter& input, UCoverX& UX, int& id)
{
    if(UX.size() == input.nodeNum)
        return input.sizes[id];
    
    int i = 0, count = 0, Size = input.sizes[id];
    for(;i < Size;i++)
        if(input.positions[input.coverages[id][i]] < -1)
            count++;
    return count;
}

void Solver::UpdateUX(PCenter& input, UCoverX& UX, int& chosenNode, int& pNumber)
{
    int chosenNodeSize = input.sizes[chosenNode], i = 0, nodei;
    for(;i < chosenNodeSize;i++){
        nodei = input.coverages[chosenNode][i];
        UX.erase(input, Uposition(input, nodei));
        input.covers[nodei].push_back(chosenNode);
    }
    input.X[pNumber] = chosenNode;
    input.positions[chosenNode] = pNumber;
}

//algorithm 2 lies here
std::pair<int, int> Solver::FindPair(PCenter& input, UCoverX& UX, int& UXSize, int& iter)
{
    
    int score, obj = MAX_VALUE, v = UX[rand(UXSize)], nodei, nodej, i, j, node_i, node_j, deltai, count = 0, vSize = input.sizes[v];

    for(i = 0;i < vSize;i++){
        nodei = input.coverages[v][i];

        if(input.tabulist[nodei] > iter)
            continue;

        this->TryToOpenCenter(input, nodei);
        deltai = - input.Delta[nodei];
        for(j = input.Xindex;j < input.centerNum;j++){
            nodej = input.X[j];

            if(input.tabulist[nodej] > iter)
                continue;

            score = deltai + input.Delta[nodej];
            // cerr << "fscore = " << fscore << " " << input.Delta[i] << " " << input.Delta[j] << endl;
            // cerr << "score = " << score << endl;

            if(score < obj){
                node_i = nodei;
                node_j = nodej;
                count = 1;
                obj = score;
            }
            else if(score == obj && rand(++count) == 0){
                node_i = nodei;
                node_j = nodej;
            }
        }
        this->RestoreDelta(input, nodei);
    }

    return std::pair<int, int>(node_i, node_j);
}

//algorithm 3 lies here
void Solver::TryToOpenCenter(PCenter& input, int i)
{
    int Size = input.sizes[i], v = 0, node;
    for(;v < Size;v++){
        node = input.coverages[i][v];
        if(input.covers[node].size() == 1){
            input.Delta[input.covers[node][0]] -= input.w[node];
        }
    }
}

void Solver::RestoreDelta(PCenter& input, int i)
{
    int Size = input.sizes[i], v = 0, node;
    for(;v < Size;v++){
        node = input.coverages[i][v];
        if(input.covers[node].size() == 1){
            input.Delta[input.covers[node][0]] += input.w[node];
        }
    }
}

//algorithm 4 lies here
void Solver::MakeMove(PCenter& input, UCoverX& UX, int& pi, int& pj)
{

    int Xpj = input.positions[pj], pSize = input.sizes[pi], nodeV, nodeL, v, l, coverSize = 0;

    for(v = 0;v < pSize;v++){

        nodeV = input.coverages[pi][v];

        coverSize = input.covers[nodeV].size();

        if(coverSize == 1){
            input.Delta[input.covers[nodeV][0]] -= input.w[nodeV];
        }
        else if(coverSize == 0){
            for(l = 0;l < input.sizes[nodeV];l++){
                nodeL = input.coverages[nodeV][l];
                if(nodeL == pi)
                    continue;
                input.Delta[nodeL] -= input.w[nodeV];
            }
        }

        UX.erase(input, Uposition(input, nodeV));

        input.covers[nodeV].push_back(pi);

    }

    input.positions[pi] = Xpj;
    input.positions[pj] = -1;
    input.X[input.positions[pi]] = pi;

    pSize = input.sizes[pj];


    for(v = 0;v < pSize;v++){

        nodeV = input.coverages[pj][v];

        input.covers[nodeV].erase(pj);

        coverSize = input.covers[nodeV].size();

        if(coverSize == 0){				//in order to judge whether 'v' is served
            UX.push_back(input, nodeV);
            for(l = 0;l < input.sizes[nodeV];l++){
                nodeL = input.coverages[nodeV][l];
                if(nodeL == pj)
                    continue;
                input.Delta[nodeL] += input.w[nodeV];
            }
        }
        else if(coverSize == 1){
            input.Delta[input.covers[nodeV][0]] += input.w[nodeV];
        }

    }

}

void Solver::solve(int*& output, PCenter& input, UCoverX& UX, function<bool()> isTimeout, int seed) 
{
    this->init(seed);

    this->calculate(output, input, UX, isTimeout);

    for (int r = 0; !isTimeout() && r < input.dropLen; r++) {
        this->reducing(input, UX, r);
        this->calculate(output, input, UX, isTimeout, r);
    }

}