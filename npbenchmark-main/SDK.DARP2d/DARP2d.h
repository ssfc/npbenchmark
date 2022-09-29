////////////////////////////////
/// usage : 1.	SDK for vehicle routing with time window on 2D plane solver.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef CN_HUST_SZX_NPBENCHMARK_DARP_2D_H
#define CN_HUST_SZX_NPBENCHMARK_DARP_2D_H


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
	Capacity demand; // customer number which is positive on a pick-up node and negtive on a delivery node.
	Time minStayTime;
	Time windowBegin;
	Time windowEnd;
};

struct DARP2d {
	static constexpr Time Precision = 1000;

	static constexpr NodeId DepotId = 0;

	NodeId requestNum;
	VehicleId maxVehicleNum;
	Capacity vehicleCapacity;
	Time maxTravelTime;
	Time maxRideTime;
	std::vector<Node2d> nodes; // `nodes[0]` is the depot. `node[i]` and `node[i + requestNum]` is a pair of pick-up and delivery request.

	NodeId nodeNum() const { return 2 * requestNum + 1; }
};

struct Route {
	std::vector<NodeId> nodes; // `nodes[n]` is the `n`th node in the route.
};

using Routes = std::vector<Route>; // `Routes[v]` is the route for vehicle `v`.


void solveDARP2d(Routes& output, DARP2d& input, std::function<bool()> isTimeout, int seed);

}


#endif // CN_HUST_SZX_NPBENCHMARK_DARP_2D_H
