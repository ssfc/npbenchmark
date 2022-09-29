////////////////////////////////
/// usage : 1.	SDK for rectangle packing solver.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef CN_HUST_SZX_NPBENCHMARK_RECT_PACKING_H
#define CN_HUST_SZX_NPBENCHMARK_RECT_PACKING_H


#include <array>
#include <vector>
#include <functional>


namespace szx {

using RectId = int;
using Coord = int;

using Rect = std::array<Coord, 2>; // `Rect[0]` is width (dx) and `Rect[1]` is height (dy).
using Coords = std::array<Coord, 2>;

struct RectPacking {
	RectId rectNum;
	std::vector<Rect> rects;
};

struct Placement {
	Coords pos; // coords of the left bottom corner.
	bool rotated; // rotate 90 degrees.
};
using Layout = std::vector<Placement>; // `Layout[r]` is the placement of rectangle `r`.


void solveRectPacking(Layout& output, RectPacking& input, std::function<bool()> isTimeout, int seed);

}


#endif // CN_HUST_SZX_NPBENCHMARK_RECT_PACKING_H
