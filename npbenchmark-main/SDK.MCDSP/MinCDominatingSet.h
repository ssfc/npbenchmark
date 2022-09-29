////////////////////////////////
/// usage : 1.	SDK for minimum connected dominating set solver.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef CN_HUST_SZX_NPBENCHMARK_MIN_C_DOMINATING_SET_H
#define CN_HUST_SZX_NPBENCHMARK_MIN_C_DOMINATING_SET_H


#include <array>
#include <vector>
#include <functional>


namespace szx {

using NodeId = int;
using EdgeId = NodeId;

using Edge = std::array<NodeId, 2>; // undirected link.

struct MinCDominatingSet {
	NodeId nodeNum;
	EdgeId edgeNum;
	std::vector<Edge> edges;
};

using Nodes = std::vector<NodeId>;


void solveMinCDominatingSet(Nodes& output, MinCDominatingSet& input, std::function<bool()> isTimeout, int seed);

}


#endif // CN_HUST_SZX_NPBENCHMARK_MIN_C_DOMINATING_SET_H
