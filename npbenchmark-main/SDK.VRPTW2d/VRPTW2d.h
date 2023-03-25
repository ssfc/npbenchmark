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

// this struct corresponds to Node.java;
struct Node2d {
	Coord2d coords; // double
	Capacity demand; // int
	Time minStayTime; // int
	Time window_begin; // int
	Time window_end; // int
};

struct VRPTW2d {
	static constexpr Time Precision = 10;

	static constexpr NodeId DepotId = 0;

    // this corresponds to the first line in the instance;
	NodeId nodeNum;
	VehicleId maxVehicleNum;
	Capacity vehicleCapacity;

    // this corresponds to the line after the first line;
	std::vector<Node2d> nodes;
};

// this corresponds to route in java class route;
struct Route {
	std::vector<NodeId> route; // `route[n]` is the `n`th node in the route.
};

// this corresponds to solution
using Routes = std::vector<Route>; // `Routes[v]` is the route for vehicle `v`.

struct Solution
{
    int total_cost;
    std::vector<Route> Routes;
};

void solveVRPTW2d(Routes& output, VRPTW2d& input, std::function<bool()> isTimeout, int seed);

}


#endif // CN_HUST_SZX_NPBENCHMARK_VRPTW_2D_H
