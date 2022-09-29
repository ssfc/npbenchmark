////////////////////////////////
/// usage : 1.	SDK for routing and wavelength assignment solver.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef CN_HUST_SZX_NPBENCHMARK_RWA_H
#define CN_HUST_SZX_NPBENCHMARK_RWA_H


#include <array>
#include <vector>
#include <functional>


namespace szx {

using NodeId = int;
using ArcId = NodeId;
using Wavelength = NodeId;
using TrafficId = NodeId;

using Arc = std::array<NodeId, 2>; // directed link.
using Traffic = Arc;

struct RWA {
	NodeId nodeNum;
	ArcId arcNum;
	TrafficId trafficNum;
	std::vector<Arc> arcs;
	std::vector<Traffic> traffics;
};

struct Route {
	Wavelength wavelen;
	std::vector<NodeId> nodes; // `nodes[n]` is the `n`th node in the route.
};

using Routes = std::vector<Route>; // `Routes[t]` is the route for traffic `t`.


void solveRWA(Routes& output, RWA& input, std::function<bool()> isTimeout, int seed);

}


#endif // CN_HUST_SZX_NPBENCHMARK_RWA_H
