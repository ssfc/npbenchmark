////////////////////////////////
/// usage : 1.	SDK for obstacle avoiding rectilinear Steiner minimum tree solver.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef CN_HUST_SZX_NPBENCHMARK_O_A_R_STEINER_MIN_TREE_H
#define CN_HUST_SZX_NPBENCHMARK_O_A_R_STEINER_MIN_TREE_H


#include <array>
#include <vector>
#include <functional>


namespace szx {

using NodeId = int;
using ObstacleId = int;
using Coord = long long;

using Coord2D = std::array<Coord, 2>; // { x, y }.
using Rect = std::array<Coord, 4>; // { xMin, yMin, xMax, yMax }.

struct OARSteinerMinTree {
	NodeId nodeNum;
	ObstacleId obstacleNum;
	std::vector<Coord2D> nodes;
	std::vector<Rect> obstacles;
};

using Path = std::vector<Coord2D>;
using Paths = std::vector<Path>;


void solveOARSteinerMinTree(Paths& output, OARSteinerMinTree& input, std::function<bool()> isTimeout, int seed);

}


#endif // CN_HUST_SZX_NPBENCHMARK_O_A_R_STEINER_MIN_TREE_H
