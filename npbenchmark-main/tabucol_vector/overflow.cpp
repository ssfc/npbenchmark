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

    test.at(0) = 1;
    test.at(1) = 2;
    test.at(30) = 30;

    cout<<"content: ";
    for(int i=0;i<test.size();i++)
    {
        cout<<test[i]<<endl;
    }

    return 0;
}