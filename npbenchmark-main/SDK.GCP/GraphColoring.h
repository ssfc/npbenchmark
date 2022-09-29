////////////////////////////////
/// usage : 1.	SDK for graph coloring solver.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef CN_HUST_SZX_NPBENCHMARK_GRAPH_COLORING_H
#define CN_HUST_SZX_NPBENCHMARK_GRAPH_COLORING_H


#include <array>
#include <vector>
#include <functional>


namespace szx {

using NodeId = int;
using EdgeId = NodeId;
using ColorId = NodeId;

using Edge = std::array<NodeId, 2>; // undirected link.

struct GraphColoring {
	NodeId nodeNum;
	EdgeId edgeNum;
	ColorId colorNum;
	std::vector<Edge> edges;
};

using NodeColors = std::vector<ColorId>; // `NodeColors[n]` is the color of node `n`.


void solveGraphColoring(NodeColors& output, GraphColoring& input, std::function<bool()> isTimeout, int seed);

}


#endif // CN_HUST_SZX_NPBENCHMARK_GRAPH_COLORING_H
