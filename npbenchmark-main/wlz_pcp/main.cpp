#include<iostream>
#include<fstream>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include "solution.h"
using namespace std;

int main(int argc, char* argv[])
{
    //Íâ²¿µ÷ÓÃÊ±²ÎÊý
    string path = argv[1];
    string output_path = argv[2];
    int limit_s = atoi(argv[3]);
    int rand_seed = atoi(argv[4]);

    VWTS sol(path, output_path);
    sol.Solve(limit_s, rand_seed);

    printf("iter:%d time:%lf  uncovered_num:%d", sol.tempiter,((double)sol.end_ms - (double)sol.start_ms) / 1000.0, sol.tempnum);
    return 0;
}