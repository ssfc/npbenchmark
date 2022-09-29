////////////////////////////////
/// usage : 1.	SDK for vehicle routing with time window on 2D plane solver.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef CN_HUST_SZX_NPBENCHMARK_VRPTW_2D_H
#define CN_HUST_SZX_NPBENCHMARK_VRPTW_2D_H


#include <array>
#include <vector>
#include <functional>


namespace szx {

using NodeId = int;
using VehicleId = int;
using Coord = double;
using Capacity = int;
using Time = int;

using Coord2d = std::array<Coord, 2>;

struct Node2d {
	Coord2d coords;
	Capacity demand;
	Time minStayTime;
	Time windowBegin;
	Time windowEnd;
};

struct VRPTW2d {
	static constexpr Time Precision = 10;

	static constexpr NodeId DepotId = 0;

	NodeId nodeNum;
	VehicleId maxVehicleNum;
	Capacity vehicleCapacity;
	std::vector<Node2d> nodes;
};

struct Route {
	std::vector<NodeId> nodes; // `nodes[n]` is the `n`th node in the route.
};

using Routes = std::vector<Route>; // `Routes[v]` is the route for vehicle `v`.


void solveVRPTW2d(Routes& output, VRPTW2d& input, std::function<bool()> isTimeout, int seed);

}


#endif // CN_HUST_SZX_NPBENCHMARK_VRPTW_2D_H
