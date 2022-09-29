#include "GraphColoring.h"

#include <random>
#include <fstream>
#include <iostream>
#include <set>

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
    cerr<<"graph: "<<num_vertex<<" "<<num_edge<<" "<<num_color<<endl;
    for(int i=0;i<adjacent.size();i++)
    {
        for(int j=0;j<adjacent[i].size();j++)
        {
            cerr<<adjacent[i][j]<<" ";
        }
        cerr<<endl;
    }
}


void Graph::add_edge(int head, int tail) 
    {
        adjacent[head][tail] = 1;
        adjacent[tail][head] = 1;
    }


int Graph::compute_conflict(vector<int> sol)
{
    int conflict = 0;
    for(int i=0;i<adjacent.size();i++)
    {
        for(int j=i+1; j<adjacent[i].size();j++)
        {
            if(adjacent[i][j] == 1 && sol[i] == sol[j])
            {
                conflict++;
            }
        }
    }

    return conflict;
}


void Graph::update_tabu_tenure_table()
{
    for(int i=0;i<num_vertex;i++)
    {
        for(int j=0;j<num_color;j++)
        {
            if(tabu_tenure_table[i][j] > 0)
            {
                tabu_tenure_table[i][j] = tabu_tenure_table[i][j] - 1;
            }
        }
    }
}


bool Graph::tabucol(int max_walks = 50, int max_iterations = 10000000) 
{     
    tabu_tenure_table.resize(num_vertex);
    for(int i=0;i<tabu_tenure_table.size();i++)
    {
        tabu_tenure_table[i].resize(num_color); 
    }

    // initialize each vertex with random color;    
//        cout<<"initial color of each vertex: ";
    for(int i=0;i<solution.size();i++)
    {            
        solution[i] = generate_random_integer(0, num_color-1);     
//            cout<<solution[i]<<' ';        
    }
//        cout<<endl;

    int aspiration_criterion = num_vertex; 
    
    int current_num_conflict; 
    for(int iteration=0; iteration<max_iterations;iteration++)
    {
        // Count vertex pairs (i,j) which are adjacent and have the same color; 
        set<int> set_conflict_vertex;
        current_num_conflict = 0; 
        for(int i=0;i<num_vertex;i++)
        {
            for(int j=i+1;j<num_vertex;j++)
            {
                if(adjacent[i][j] == 1 && solution[i] == solution[j]) // vertices belongs to one edge have same color; 
                {
                    set_conflict_vertex.insert(i);
                    set_conflict_vertex.insert(j);
                    current_num_conflict++; 
                }
            }
        }          

        if(current_num_conflict == 0) // Found a solution.
        {
            break; 
        }  

        // convert conflict vertex from set to vector; 
        vector<int> vector_conflict_vertex(set_conflict_vertex.begin(), set_conflict_vertex.end());
        vector<int> new_solution(num_vertex, -1);
        int vertex_changed_idx; 
        int vertex_changed; 
        int new_color; 

        for(int step=0; step<max_walks; step++)
        {
//            cout<<"step: "<<step<<endl; 
            // Choose a random vertex to change; 
            vertex_changed_idx = generate_random_integer(0, vector_conflict_vertex.size()-1); 
            vertex_changed = vector_conflict_vertex[vertex_changed_idx];

            // Choose a new color; 
            int new_color = generate_random_integer(0, num_color-1);
            while(solution[vertex_changed] == new_color)
            {
                new_color = generate_random_integer(0, num_color-1);
            }

            // Create a neighbor solution; 
            new_solution = solution;
            new_solution[vertex_changed] = new_color; 

            // Count adjacent pairs with the same color in the new solution.
            int new_num_conflict = compute_conflict(new_solution); 
            if(new_num_conflict < current_num_conflict)  // found an improved solution
            {                    
                if(new_num_conflict < aspiration_criterion)  // better than aspiration; 
                {
                    aspiration_criterion = new_num_conflict; 
                    
                    // permit tabu move if it is better than previous best; 
                    if(tabu_tenure_table[vertex_changed][new_color] > 0)  
                    {
                        tabu_tenure_table[vertex_changed][solution[vertex_changed]] = current_num_conflict + generate_random_integer(1,10); 
                        cerr<<"satisfy aspiration, tabu released: "<<current_num_conflict<<"->"<<new_num_conflict<<endl; 
                        cerr<<"Iteration "<<iteration<<": "<<current_num_conflict<<"->"<<new_num_conflict<<endl;
                        break; 
                    }
                    else 
                    {
                        tabu_tenure_table[vertex_changed][solution[vertex_changed]] = current_num_conflict + generate_random_integer(1,10); 
                        cerr<<"Iteration "<<iteration<<": "<<current_num_conflict<<"->"<<new_num_conflict<<endl;
                        break; 
                    }
                }
                else // not better than aspiration; 
                {                      
                    if(tabu_tenure_table[vertex_changed][new_color] > 0)
                    {
                        // tabu move does not satisfy aspiration; 
                        cerr<<"although better, not better than aspiration, tabu forbidden"<<endl;
                    }
                    else
                    {
                        tabu_tenure_table[vertex_changed][solution[vertex_changed]] = current_num_conflict + generate_random_integer(1,10); 
                        cerr<<"Iteration "<<iteration<<": "<<current_num_conflict<<"->"<<new_num_conflict<<endl;
                        break; 
                    }
                }
            }
        }
        
        solution = new_solution; 
        update_tabu_tenure_table(); 
    }

    if(current_num_conflict != 0)
        return false;
    else
        cerr<<"find answer for color number "<<num_color<<": "<<endl;
        for(int i=0;i<num_vertex;i++)
        {
            cerr<<i<<"->"<<solution[i]<<" ";            
        }
        cerr<<endl;
        
        return true;
}


void Graph::save_vertex_color() // save color of each vertex; 
{
    ofstream outFile("solution.txt");
    // the important part
    for (const auto &e : solution) outFile << e << "\n";
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

        
		Graph g_test;
		g_test.initialize_graph(input.nodeNum, input.edgeNum, input.colorNum);

		for(int i=0;i<input.edgeNum;i++)
		{
			g_test.add_edge(input.edges[i][0], input.edges[i][1]);
		}

		// g_test.print_graph();

		g_test.tabucol();
		g_test.save_vertex_color();

        /*
	    int num_vertex; // num of vertices; 
        int num_edge; // num of edges; 
        int num_color; // num of colors;    
	    vector<vector<int> > adjacent; 
        vector<int> solution; 
        vector<vector<int>> tabu_tenure_table; 

        // initialize graph; 
		num_vertex = input.nodeNum; 
		num_edge = input.edgeNum;
		num_color = input.colorNum;
		// num_color = 70;

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

        // add edge; 
		for(int i=0;i<input.edgeNum;i++)
		{
			adjacent[input.edges[i][0]][input.edges[i][1]] = 1;
			adjacent[input.edges[i][1]][input.edges[i][0]] = 1;
		}

		cerr<<"graph: "<<num_vertex<<" "<<num_edge<<" "<<num_color<<endl;
		for(int i=0;i<adjacent.size();i++)
		{
			for(int j=0;j<adjacent[i].size();j++)
			{
				cerr<<adjacent[i][j]<<" ";
			}
			cerr<<endl;
		}
        */

		




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
