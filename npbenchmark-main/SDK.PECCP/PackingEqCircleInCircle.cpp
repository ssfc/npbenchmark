#include "PackingEqCircleInCircle.h"

#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "SvgDrawer.h"


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
	void solve(Layout& output, PackingEqCircleInCircle& input, std::function<bool()> isTimeout, int seed) {
		initRand(seed);

		// TODO: implement your own solver which fills the `output` to replace the following trivial solver.
		// sample solver: place circles randomly (the solution can be infeasible).

		Coord x = 0;
		//                        +----[ exit before timeout ]
		//                        |
		for (CircleId c = 0; !isTimeout() && (c < input.circleNum); ++c) {
			//               +----[ use the random number generator initialized by the given seed ]
			//               |
			output[c][0] = rand(-9, 10);
			output[c][1] = rand(-9, 10);
		}

		// TODO: the following code in this function is for illustration only and can be deleted.
		draw(input, output);
		// print some information for debugging.
		cerr << input.circleNum << endl;
		cerr << "x\ty" << endl;
		for (CircleId c = 0; !isTimeout() && (c < input.circleNum); ++c) { cerr << output[c][0] << '\t' << output[c][1] << endl; }
	}

	static void draw(const PackingEqCircleInCircle& input, const Layout& output, const std::string filePath = "peccp.visualization.html") {
		Coord r = 0;
		goal::SvgDrawer<Coord> sd;
		for (CircleId c = 0; c < input.circleNum; ++c) {
			Coord d = output[c][0] * output[c][0] + output[c][1] * output[c][1];
			if (d > r) { r = d; }
			sd.circle(output[c][0], output[c][1], 1, to_string(c));
		}
		r = sqrt(r) + 1;
		sd.circle(0, 0, r);
		ofstream ofs(filePath);
		ofs << sd.toStr(-r - 1, -r - 1, 2 * r + 2, 2 * r + 2, 1080, 1080, "", "",
			"<style>circle { stroke:black; stroke-width: 1px; fill:transparent; vector-effect: non-scaling-stroke; } text { vector-effect: non-scaling-stroke; }</style>") << endl;
	}
};

// solver.
void solvePackingEqCircleInCircle(Layout& output, PackingEqCircleInCircle& input, std::function<bool()> isTimeout, int seed) {
	Solver().solve(output, input, isTimeout, seed);
}

}
