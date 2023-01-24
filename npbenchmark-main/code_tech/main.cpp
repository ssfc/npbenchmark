#include <iostream>

using namespace std;

class Example {
public:
    int x;
    double y;
    string z;
};

int main()
{
    Example test;

    cout << "x: " << test.x << endl;
    cout << "y: " << test.y << endl;
    cout << "z: " << test.z << endl;

    return 0;
}
