////////////////////////////////
/// usage : 1.	SDK for rectangle packing solver.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef CN_HUST_SZX_NPBENCHMARK_PACKING_EQ_CIRCLE_IN_CIRCLE_H
#define CN_HUST_SZX_NPBENCHMARK_PACKING_EQ_CIRCLE_IN_CIRCLE_H


#include <array>
#include <vector>
#include <functional>


namespace szx {

using CircleId = int;
using Coord = double;

using Coords = std::array<Coord, 2>;

struct PackingEqCircleInCircle {
	CircleId circleNum;
	Coord ratio; // best known (ContainerRadius / CircleRadius).
	Coord err; // error tolerance for overlap/overflow checking.
};

using Layout = std::vector<Coords>; // `Layout[c]` is the coordinates of the center of circle `c`.


void solvePackingEqCircleInCircle(Layout& output, PackingEqCircleInCircle& input, std::function<bool()> isTimeout, int seed);

}


#endif // CN_HUST_SZX_NPBENCHMARK_PACKING_EQ_CIRCLE_IN_CIRCLE_H
