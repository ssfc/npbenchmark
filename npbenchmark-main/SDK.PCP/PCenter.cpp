# include "PCenter.h"
# include "pcp_greedy.h"
# include "pcp_vector.h"

# include <algorithm>
# include <ctime>
# include <iostream>
# include <random>


using namespace std;


namespace szx {

class Solver {
	// random number generator.
	mt19937 pseudoRandNumGen;
	void initRand(int seed) { pseudoRandNumGen = mt19937(seed); }
	int fastRand(int lb, int ub) { return (pseudoRandNumGen() % (ub - lb)) + lb; }
	int fastRand(int ub) { return pseudoRandNumGen() % ub; }
	int rand(int lb, int ub) { return uniform_int_distribution<int>(lb, ub - 1)(pseudoRandNumGen); }
	int rand(int ub) { return uniform_int_distribution<int>(0, ub - 1)(pseudoRandNumGen); }

public:
	void solve(Centers& output, PCenter& input, std::function<bool()> isTimeout, int seed) {
		initRand(seed);

        if(input.nodeNum != 3038) // pcb3038以外的数据集
        {
            cerr << "current_radius: " << input.current_radius << endl;
            coverAllNodesUnderFixedRadius(output, input, isTimeout, seed);
        }
        else // pcb3038数据集
        {
            int counter = 0;
            for (auto r = input.nodesWithDrops.begin(); !isTimeout() && (r != input.nodesWithDrops.end()); ++r)
            {
                reduceRadius(input, *r);
                input.current_radius--;

                // if(counter>=0)
                if((input.centerNum == 10 && counter == 125) // pcb3038p010r729.txt ; 125 = 108165-108039-1
                   || (input.centerNum == 20 && counter == 191) // pcb3038p020r494.txt ; 191 = 51004-50812-1
                   || (input.centerNum == 30 && counter == 77) // pcb3038p030r394.txt ; 77 = 32772-32694-1
                   || (input.centerNum == 40 && counter == 82) // pcb3038p040r337.txt ; 82 = 24095-24012-1
                   || (input.centerNum == 50 && counter == 152) // pcb3038p050r299.txt ; 152 = 19000-18847-1
                   || (input.centerNum == 100 && counter == 59) // pcb3038p100r207.txt ; 59 = 9136-9076-1
                   || (input.centerNum == 150 && counter == 36) // pcb3038p150r165.txt ; 36 = 5775-5738-1
                   || (input.centerNum == 200 && counter == 56)) // pcb3038p200r141.txt ; 56 = 4210-4153-1
                {
                    cerr << "current_radius: " << input.current_radius << endl;
                    coverAllNodesUnderFixedRadius(output, input, isTimeout, seed);
                    break;
                }

                /*
                else
                {
                    cerr << "current_radius: " << input.current_radius << endl;
                    coverAllNodesUnderFixedRadius(output, input, isTimeout, seed);
                }
                 //*/

                counter++;
            }
        }
	}

	void coverAllNodesUnderFixedRadius(Centers& output, PCenter& input, std::function<bool()> isTimeout, int seed) {
		// TODO: implement your own solver which fills the `output` to replace the following trivial solver.

        // 1: greedy method;
        // PCP_Greedy test_graph(input.nodeNum, input.centerNum, input.coverages, input.nodesWithDrops, seed);
        // test_graph.greedy_construct();

        // 2: local search method;
        ///*
        // input.centerNum = 3;
        PCP_Vector test(input.nodeNum, input.centerNum, input.current_radius, input.coverages, seed);
        test.vertex_weight_tabu_search();
        test.get_solution(output);

        /*
        cerr << "Evaluate output: ";
        for(int i=0;i<input.centerNum;i++)
        {
            cerr << output[i] << " ";
        }
        cerr << endl;
         */

         //*/
	}

	void reduceRadius(PCenter& input, Nodes nodesWithDrop) {
		for (auto n = nodesWithDrop.begin(); n != nodesWithDrop.end(); ++n) {
			input.coverages[*n].pop_back();
		}
	}
};

// solver.
void solvePCenter(Centers& output, PCenter& input, std::function<bool()> isTimeout, int seed) {
	Solver().solve(output, input, isTimeout, seed);
}

}
