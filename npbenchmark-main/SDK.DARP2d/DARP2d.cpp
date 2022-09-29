#include "DARP2d.h"

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
	static Time travelTime(const Coord2d& src, const Coord2d& dst) {
		return static_cast<Time>(hypot(src[0] - dst[0], src[1] - dst[1]) * DARP2d::Precision);
	}

	void solve(Routes& output, DARP2d& input, std::function<bool()> isTimeout, int seed) {
		initRand(seed);

		// use fix-point real number as time unit.
		vector<vector<Time>> travelTimes(input.nodeNum(), vector<Time>(input.nodeNum()));
		for (NodeId n = 0; n < input.nodeNum(); ++n) {
			travelTimes[n][n] = 0;
			for (NodeId m = 0; m < n; ++m) {
				travelTimes[n][m] = travelTimes[m][n] = travelTime(input.nodes[n].coords, input.nodes[m].coords);
			}
		}
		input.maxTravelTime *= DARP2d::Precision;
		input.maxRideTime *= DARP2d::Precision;
		for (auto n = input.nodes.begin(); n != input.nodes.end(); ++n) {
			n->minStayTime *= DARP2d::Precision;
			n->windowBegin *= DARP2d::Precision;
			n->windowEnd *= DARP2d::Precision;
		}

		// TODO: implement your own solver which fills the `output` to replace the following trivial solver.
		// sample solver: add nodes to routes randomly (the solution can be infeasible).

		VehicleId vehicleNum = input.maxVehicleNum - rand(input.maxVehicleNum / 2);
		//                       |
		//                       +----[ use the random number generator initialized by the given seed ]
		//                      +----[ exit before timeout ]
		//                      |
		for (NodeId n = 1; !isTimeout() && (n < input.nodeNum()); ++n) {
			VehicleId v = rand(vehicleNum);
			output[v].nodes.push_back(n);
		}

		// TODO: the following code in this function is for illustration only and can be deleted.
		// print some information for debugging.
		cerr << input.nodeNum() << '\t' << input.maxVehicleNum << '\t' << input.vehicleCapacity << endl;
		cerr << "vehicle\tnodes" << endl;
		for (VehicleId v = 0; !isTimeout() && (v < vehicleNum); ++v) { cerr << v << '\t' << output[v].nodes.size() << endl; }
	}
};

// solver.
void solveDARP2d(Routes& output, DARP2d& input, std::function<bool()> isTimeout, int seed) {
	Solver().solve(output, input, isTimeout, seed);
}

}
