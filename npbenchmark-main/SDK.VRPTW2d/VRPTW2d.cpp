#include "AntColony.h"
#include "VRPTW2d.h"

#include <random>
#include <iostream>

#include <cmath>


using namespace std;


namespace szx {

class Solver {
	// random number generator.
	mt19937 pseudoRandNumGen;
	void initRand(int seed) { pseudoRandNumGen = mt19937(seed); }
	int fastRand(int lb, int ub) { return (pseudoRandNumGen() % (ub - lb)) + lb; }
	int fastRand(int ub) { return pseudoRandNumGen() % ub; }
	int rand(int lb, int ub) { return uniform_int_distribution<int>(lb, ub - 1)(pseudoRandNumGen); }
	int rand(int ub) { return uniform_int_distribution<int>(0, ub - 1)(pseudoRandNumGen); }

public:
    // 车辆在两个顶点之间的旅行时间
	static Time travelTime(const Coord2d& src, const Coord2d& dst)
    {
		return static_cast<Time>(hypot(src[0] - dst[0], src[1] - dst[1]) * VRPTW2d::Precision);
	}

	void solve(Routes& output, VRPTW2d& input, std::function<bool()> isTimeout, int seed)
    {
		initRand(seed);

		// TODO: implement your own solver which fills the `output` to replace the following trivial solver.
		// sample solver: add nodes to routes randomly (the solution can be infeasible).

        // compute distance between two nodes;
		vector<vector<Time>> travelTimes(input.nodeNum, vector<Time>(input.nodeNum));
		for (NodeId n = 0; n < input.nodeNum; ++n)
        {
			travelTimes[n][n] = 0;
			for (NodeId m = 0; m < n; ++m)
            {
				travelTimes[n][m] = travelTimes[m][n] = travelTime(input.nodes[n].coords, input.nodes[m].coords);
			}
		}

		for (auto & node : input.nodes)
        {
			node.minStayTime *= VRPTW2d::Precision; // *= 不就是原值*10的意思嘛
			node.windowBegin *= VRPTW2d::Precision;
			node.windowEnd *= VRPTW2d::Precision;
		}

        AntColony test(input, travelTimes, seed);
        test.ACS_Strategy();

	}
};

// solver.
void solveVRPTW2d(Routes& output, VRPTW2d& input, std::function<bool()> isTimeout, int seed) {
	Solver().solve(output, input, isTimeout, seed);
}

}
