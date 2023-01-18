//
// This file does not affect the running of project, only for debug and learn;
//

#include <iostream>
#include <vector>

using namespace std;


int main()
{
    vector<int> test;
    test.resize(2);

    test[30] = 4;
    cout<<test[30]<<endl;

    cout<<"content: ";
    for(int i=0;i<test.size();i++)
    {
        cout<<test[i]<<endl;
    }

    return 0;
}