//
// Created by take_ on 2023/1/14.
//

#include <iostream>

using namespace std;

void hello()
{
    cout<<"hello world"<<endl;
}


int main()
{
    int arr[4] = {1, 2, 3, 4};
    for(int i : arr)
    {
        cout<<i<<endl;
    }

    hello();

    return 0;
}