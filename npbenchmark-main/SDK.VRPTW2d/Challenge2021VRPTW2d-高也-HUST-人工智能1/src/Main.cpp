#include <iostream>
#include <string>
#include <chrono>

#include "VRPTW2d.h"


using namespace std;
using namespace szx;


void loadInput(istream& is, VRPTW2d& vrp)
{
    // 使用了istream的>>操作符，将输入流中的前三个数分别赋值给vrp对象的nodeNum、maxVehicleNum和vehicleCapacity成员变量。
	is >> vrp.nodeNum >> vrp.maxVehicleNum >> vrp.vehicleCapacity;
	vrp.nodes.resize(vrp.nodeNum);
	for (auto & node : vrp.nodes)
    {
        // 使用了>>操作符，将输入流中的节点坐标、需求量、最小停留时间、时间窗口开始时间和结束时间分别赋值给了节点的对应成员变量。
		is >> node.coords[0] >> node.coords[1] >> node.demand >> node.min_stay_time >> node.window_begin >> node.window_end;
	}
}

void saveOutput(ostream& os, Routes& routes)
{
	for (auto & route : routes)
    {
		if (route.route.empty())
        {
            continue;
        }

		for (int & node : route.route)
        {
            os << node << ' ';
        }

		os << endl;
	}
}

void test(istream& inputStream, ostream& outputStream, long long secTimeout, int randSeed)
{
	cerr << "load input." << endl;
	VRPTW2d vrp;
	loadInput(inputStream, vrp);

	cerr << "solve." << endl;
    // 调用 chrono::steady_clock::now() 方法来获取当前时间点。
	chrono::steady_clock::time_point endTime = chrono::steady_clock::now() + chrono::seconds(secTimeout);
	Routes routes(vrp.maxVehicleNum);
    // 定义了一个 lambda 函数，使用了捕获列表 [&]，捕获了当前作用域中的所有变量，并将其作为引用传递给 lambda 函数。lambda 函数的返回类型是 bool，即返回一个布尔值。
	solveVRPTW2d(routes, vrp, [&]() -> bool { return endTime < chrono::steady_clock::now(); }, randSeed);

	cerr << "save output." << endl;
	saveOutput(outputStream, routes);
}

void test(istream& inputStream, ostream& outputStream, long long secTimeout)
{
	return test(inputStream, outputStream, secTimeout, static_cast<int>(time(nullptr) + clock()));
}


int main(int argc, char* argv[])
{
	cerr << "load environment." << endl;
	if (argc > 2)
    {
        char* endptr;
        long long secTimeout = strtoll(argv[1], &endptr, 10);
        if (*endptr != '\0')
        {
            std::cerr << "Error: Invalid argument '" << argv[1] << "'\n";
            return 1;
        }

        long randSeed = strtol(argv[2], &endptr, 10);
        if (*endptr != '\0')
        {
            std::cerr << "Error: Invalid argument '" << argv[2] << "'\n";
            return 1;
        }

		test(cin, cout, secTimeout, randSeed);
	}
    else
    {
		//ifstream ifs("path/to/instance.txt");
		//ofstream ofs("path/to/solution.txt");
		//test(ifs, ofs, 10); // for self-test.
	}

	return 0;
}

/*
 * laptop path:
 * C:\wamp64\www\npbenchmark\npbenchmark-main\SDK.VRPTW2d
 * home server path:
 * /home/ssfc/SDK.VRPTW2d
 * Compiling and running command:
 * home server:
 * g++ Main.cpp VRPTW2d.cpp AntColony.cpp -O3 && ./a.out 2 1 <./data/solomon.c101.n101v25c200.txt  >sln.txt
 * pack and submit:
 * g++ -static-libgcc -static-libstdc++ Main.cpp VRPTW2d.cpp AntColony.cpp -O3
 */