#include "RectPacking.h"

#include <random>
#include <iostream>
#include <fstream>

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
	void solve(Layout& output, RectPacking& input, std::function<bool()> isTimeout, int seed) {
		initRand(seed);

		// TODO: implement your own solver which fills the `output` to replace the following trivial solver.
		// sample solver: place rectangles randomly (the solution can be infeasible).

		Coord x = 0;
		//                      +----[ exit before timeout ]
		//                      |
		for (RectId r = 0; !isTimeout() && (r < input.rectNum); ++r) {
			//                    +----[ use the random number generator initialized by the given seed ]
			//                    |
			output[r].rotated = rand(2) & 1;
			output[r].pos[0] = x;
			x += (output[r].rotated ? input.rects[r][1] : input.rects[r][0]);
			output[r].pos[1] = 0;
		}

		// TODO: the following code in this function is for illustration only and can be deleted.
		draw(input, output);
		// print some information for debugging.
		cerr << input.rectNum << endl;
		cerr << "x\ty\trotated" << endl;
		for (RectId r = 0; !isTimeout() && (r < input.rectNum); ++r) { cerr << output[r].pos[0] << '\t' << output[r].pos[1] << '\t' << output[r].rotated << endl; }
	}

	static void draw(const RectPacking& input, const Layout& output, const std::string filePath = "rpp.visualization.html") {
		Coord minX = output[0].pos[0];
		Coord maxX = output[0].pos[0];
		Coord minY = output[0].pos[1];
		Coord maxY = output[0].pos[1];
		goal::SvgDrawer<Coord> sd;
		for (RectId r = 0; r < input.rectNum; ++r) {
			Coord w = !output[r].rotated ? input.rects[r][0] : input.rects[r][1];
			Coord h = output[r].rotated ? input.rects[r][0] : input.rects[r][1];
			if (output[r].pos[0] < minX) { minX = output[r].pos[0]; }
			if ((output[r].pos[0] + w) > maxX) { maxX = output[r].pos[0] + w; }
			if (output[r].pos[1] < minY) { minY = output[r].pos[1]; }
			if ((output[r].pos[1] + h) > maxY) { maxY = output[r].pos[1] + h; }
			sd.rect(output[r].pos[0], output[r].pos[1], w, h);
		}
		ofstream ofs(filePath);
		double aspectRatio = static_cast<double>(maxX - minX) / static_cast<double>(maxY - minY);
		ofs << sd.toStr(minX - 10, minY - 10, maxX - minX + 20, maxY - minY + 20, 960 * aspectRatio, 960 / aspectRatio, "", "",
			"<style>rect { stroke:black; stroke-width: 1px; fill:transparent; }</style>") << endl;
	}
};

// solver.
void solveRectPacking(Layout& output, RectPacking& input, std::function<bool()> isTimeout, int seed) {
	Solver().solve(output, input, isTimeout, seed);
}

}
