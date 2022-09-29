////////////////////////////////
/// usage : 1.	SDK for flexible jobshop scheduling solver.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef CN_HUST_SZX_NPBENCHMARK_FJSP_H
#define CN_HUST_SZX_NPBENCHMARK_FJSP_H


#include <array>
#include <vector>
#include <functional>


namespace szx {

using JobId = int;
using WorkerId = JobId;
using OperationId = JobId;
using Duration = int;

struct Candidate {
	WorkerId worker;
	Duration duration;
};

using Operation = std::vector<Candidate>;

using Job = std::vector<Operation>;

struct FJSP {
	JobId jobNum;
	WorkerId workerNum;
	WorkerId maxCandidateNum;
	std::vector<Job> jobs;
};

struct Task {
	JobId job;
	OperationId operation;
};
using Tasks = std::vector<Task>;
using Schedule = std::vector<Tasks>; // `Schedule[w][t]` is the `t`th task assigned to worker `w`.


void solveFJSP(Schedule& output, FJSP& input, std::function<bool()> isTimeout, int seed);

}


#endif // CN_HUST_SZX_NPBENCHMARK_FJSP_H
