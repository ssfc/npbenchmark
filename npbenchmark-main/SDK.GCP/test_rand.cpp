#include <random>
#include <iostream>

using namespace std; 


int generate_random_integer(int lower, int upper, int seed)
{
    srand(seed);
    int result = (rand() % (upper - lower + 1)) + lower;

    return result;
}


int main()
{
    int seed = 234; 
    for (int i = 0;i < 10;i++)
    {
        cout << generate_random_integer(1, 5, seed+i) << endl;
    }

	return 0; 
}






