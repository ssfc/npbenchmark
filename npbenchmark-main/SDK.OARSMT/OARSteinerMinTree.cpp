#include "OARSteinerMinTree.h"

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
	void solve(Paths& output, OARSteinerMinTree& input, std::function<bool()> isTimeout, int seed) {
		initRand(seed);

		// TODO: implement your own solver which fills the `output` to replace the following trivial solver.
		// sample solver: connect nodes in turn ordered by their index (the solution can be infeasible).

		Path path = { input.nodes[0] };
		//                      +----[ exit before timeout ]
		//                      |
		for (NodeId n = 1; !isTimeout() && (n < input.nodeNum); ++n) {
			if (input.nodes[n] == path.back()) { continue; }
			//    +----[ use the random number generator initialized by the given seed ]
			//    |
			if (rand(2) == 0) {
				path.push_back({ path.back()[0], input.nodes[n][1] }); // move along y axis.
			} else {
				path.push_back({ input.nodes[n][0], path.back()[1] }); // move along x axis.
			}
			path.push_back(input.nodes[n]);
		}
		output.push_back(path);
		
		// TODO: the following code in this function is for illustration only and can be deleted.
		draw(input, output);
		// print some information for debugging.
		NodeId maxDebugLineNum = 32;
		for (auto path = output.begin(); !isTimeout() && (path != output.end()); ++path) {
			for (auto p = path->begin(); p != path->end(); ++p) {
				cerr << (*p)[0] << '\t' << (*p)[1] << endl;
				if (maxDebugLineNum-- < 0) { return; }
			}
			cerr << "----" << endl;
		}
	}

	static void draw(const OARSteinerMinTree& input, const Paths& output, const std::string filePath = "oarsmt.visualization.html") {
		Coord minX = input.nodes[0][0];
		Coord maxX = input.nodes[0][0];
		Coord minY = input.nodes[0][1];
		Coord maxY = input.nodes[0][1];
		goal::SvgDrawer<Coord> sd;
		for (NodeId n = 0; n < input.nodeNum; ++n) { sd.circle(input.nodes[n][0], input.nodes[n][1], 10); }
		for (ObstacleId o = 0; o < input.obstacleNum; ++o) {
			const Rect& r(input.obstacles[o]);
			sd.rect(r[0], r[1], r[2] - r[0], r[3] - r[1]);
		}
		for (auto p = output.begin(); p != output.end(); ++p) {
			sd.pathPoly(*p);
			for (auto n = p->begin(); n != p->end(); ++n) {
				if ((*n)[0] < minX) { minX = (*n)[0]; } else if ((*n)[0] > maxX) { maxX = (*n)[0]; }
				if ((*n)[1] < minY) { minY = (*n)[1]; } else if ((*n)[1] > maxY) { maxY = (*n)[1]; }
			}
		}
		ofstream ofs(filePath);
		ofs << sd.toStr(minX - 10, minY - 10, maxX - minX + 20, maxY - minY + 20, 1920, 1080, "", "",
			"<style>rect { fill:grey; stroke:red; stroke-width: 1px; } line { stroke:black; stroke-width: 1px; } path { stroke: black; fill:transparent; }</style>") << endl;
	}
};

// solver.
void solveOARSteinerMinTree(Paths& output, OARSteinerMinTree& input, std::function<bool()> isTimeout, int seed) {
	Solver().solve(output, input, isTimeout, seed);
}

}
