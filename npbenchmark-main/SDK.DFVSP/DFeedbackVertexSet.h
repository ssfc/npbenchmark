////////////////////////////////
/// usage : 1.	SDK for directed feedback vertex set problem solver.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef CN_HUST_SZX_NPBENCHMARK_D_FEEDBACK_VERTEX_SET_H
#define CN_HUST_SZX_NPBENCHMARK_D_FEEDBACK_VERTEX_SET_H


#include <array>
#include <vector>
#include <functional>


namespace szx {

using NodeId = int;
using ArcId = NodeId;

using AdjNodes = std::vector<NodeId>; // `AdjNodes[i]` is the `i`th adjacent node.
using AdjList = std::vector<AdjNodes>; // `AdjList[n]` is the adjacent nodes of node `n`.

struct DFeedbackVertexSet {
	NodeId nodeNum;
	ArcId arcNum;
	AdjList adjList;
};

using Nodes = std::vector<NodeId>;


void solveDFeedbackVertexSet(Nodes& output, DFeedbackVertexSet& input, std::function<bool()> isTimeout, int seed);

}


#endif // CN_HUST_SZX_NPBENCHMARK_D_FEEDBACK_VERTEX_SET_H
