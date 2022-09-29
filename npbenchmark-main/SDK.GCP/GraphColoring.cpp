#include "GraphColoring.h"

#include <random>
#include <iostream>


using namespace std;


int generate_random_integer(int lower, int upper)
{
    random_device rd; // obtain a random number from hardware
    mt19937 gen(rd()); // seed the generator
    uniform_int_distribution<> distr(lower, upper); // define the range 

    return distr(gen);
}


class Graph
{
private: 
	int num_vertex; // num of vertices; 
    int num_edge; // num of edges; 
    int num_color; // num of colors;    
	vector<vector<int> > adjacent; 
    vector<int> solution; 
    vector<vector<int>> tabu_tenure_table; 
public:   
	void initialize_graph(int input_num_vertex, int input_num_edge, int input_num_color); // initialize variables; 
    void print_graph(); // print adjacent matrix of graph; 
	void add_edge(int head, int tail);  // function to add an edge to graph; 

    int compute_conflict(vector<int> sol); 
    void update_tabu_tenure_table(); 
	bool tabucol(int max_walks, int max_iterations); // do tabucol for graph; 

    void save_vertex_color(); 
};


void Graph::initialize_graph(int input_num_vertex, int input_num_edge, int input_num_color) 
{
    num_vertex = input_num_vertex; 
    num_edge = input_num_edge;
//        num_color = input_num_color;
    num_color = 70;

    adjacent.resize(num_vertex); // initialize adjacent matrix; 
    for(int i=0;i<adjacent.size();i++)
    {
        adjacent[i].resize(num_vertex);
    }

    solution.resize(num_vertex); // initialize solution; 
    for(int i=0;i<solution.size();i++)
    {
        solution[i] = -1;
    }
}


void Graph::print_graph() 
{
    cout<<"graph: "<<num_vertex<<" "<<num_edge<<" "<<num_color<<endl;
    for(int i=0;i<adjacent.size();i++)
    {
        for(int j=0;j<adjacent[i].size();j++)
        {
            cout<<adjacent[i][j]<<" ";
        }
        cout<<endl;
    }
}





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
	void solve(NodeColors& output, GraphColoring& input, std::function<bool()> isTimeout, int seed) {
		initRand(seed);

		// TODO: implement your own solver which fills the `output` to replace the following trivial solver.
		// sample solver: assign colors randomly (the solution can be infeasible).

		//                      +----[ exit before timeout ]
		//                      |
		for (NodeId n = 0; !isTimeout() && (n < input.nodeNum); ++n) 
		{ 
			output[n] = rand(input.colorNum); 
		}
		//                                                                           |
		//      [ use the random number generator initialized by the given seed ]----+

		// TODO: the following code in this function is for illustration only and can be deleted.
		// print some information for debugging.

		
		cerr << "node Num: " << input.nodeNum << endl;
		cerr << "edge Num: " << input.edgeNum << endl;
		cerr << "color Num: " << input.colorNum << endl;
		cerr << "first edge: "<< input.edges[0][0] << " " << input.edges[0][1] << endl; 
		cerr << "second edge: "<< input.edges[1][0] << " " << input.edges[1][1] << endl; 


		/*
		cerr << "node\tcolor" << endl;
		for (NodeId n = 0; !isTimeout() && (n < input.nodeNum); ++n) { cerr << n << '\t' << output[n] << endl; }
		*/
	}
};

// solver.
void solveGraphColoring(NodeColors& output, GraphColoring& input, std::function<bool()> isTimeout, int seed) {
	Solver().solve(output, input, isTimeout, seed);
}

}
