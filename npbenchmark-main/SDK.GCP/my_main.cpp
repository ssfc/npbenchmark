//
// Created by take_ on 2022/11/29.
//
# include <iostream>

using namespace std;

int main()
{
    cout << "start allocating static array" << endl;
    int equ_delta[2000][2];
    for(int i=0;i<2000;i++)
    {
        for(int j=0;j<2;j++)
        {
            equ_delta[i][j] = i+j;
        }
    }

    cout<<equ_delta[1996][1]<<endl;

    cout << "finish allocating static array" << endl;

    return 0;
}



