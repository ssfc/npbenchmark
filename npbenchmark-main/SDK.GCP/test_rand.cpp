#include <random>
#include <iostream>

using namespace std; 


int generate_random_integer(int lower, int upper, int seed)
{
    srand(seed);
    int result = (rand() % (upper - lower + 1)) + lower;

    return result;
}

// random number generator.
mt19937 pseudoRandNumGen;
void initRand(int seed) { pseudoRandNumGen = mt19937(seed); }
int fastRand(int lb, int ub) { return (pseudoRandNumGen() % (ub - lb)) + lb; }
int fastRand(int ub) { return pseudoRandNumGen() % ub; }
int rand(int lb, int ub) { return uniform_int_distribution<int>(lb, ub - 1)(pseudoRandNumGen); }
int rand(int ub) { return uniform_int_distribution<int>(0, ub - 1)(pseudoRandNumGen); }


int main()
{
    int seed = 234;
    initRand(seed);

    for (int i = 0;i < 20;i++)
    {
        // cout << generate_random_integer(1, 5, seed+i) << endl;
        cout << fastRand(5) << endl;
    }

	return 0; 
}






