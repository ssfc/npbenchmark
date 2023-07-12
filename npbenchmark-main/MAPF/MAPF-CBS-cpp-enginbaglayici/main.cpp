// ConflictBasedSearch.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <chrono>
#include "LowLevelSolver.h"
#include "HighLevelSolver.h"

using namespace std;

vector<string> split(string s, char delim)
{
	vector<string> result;
	stringstream ss(s);
	string item;

	while (getline(ss, item, delim))
    {
		result.push_back(item);
	}

	return result;
}

Map read_map(string filename)
{
	Map map; // Grid map
	int row, col;

	ifstream infile(filename);
	string line;

	getline(infile, line);

	auto splitted = split(line, ' ');
	row = stoi(splitted[0]);
	col = stoi(splitted[1]);

	getline(infile, line);

	// split second line to get obstacles
	istringstream k(line);
	string s;

	vector<vector<Cell>> cells;

	for (int i = 0; i < row; i++)
    {
		vector<Cell> row;
		for (int j = 0; j < col; j++)
        {
			row.emplace_back(Cell(i, j));
		}

		cells.emplace_back(row);
	};

	while (getline(k, s, ' '))
    {
		int obstacleIndex = stoi(s);
		int x = obstacleIndex % row;
		int y = obstacleIndex / row;

		cells[x][y].isObstacle = true;
	}

	// next lines are agents. startIndex endIndex
	int start, end;

	vector<Agent> agents;

	int agentID = 0;
	while (infile >> start >> end)
    {
		int startX = start % row;
		int startY = start / row;

		int endX = end % row;
		int endY = end / row;

		Agent agent(agentID);
		agent.start = Cell(startX, startY);
		agent.end = Cell(endX, endY);
		cerr << "start: "  << startX << startY;
		cerr << " end: " << endX << endY << "\n";
		agents.emplace_back(agent);
		agentID++;
	}

	map.cells = cells;
	map.agents = agents;

	return map;
}

void print_map(Map map)
{
	for (int i = 0; i < map.cells.size(); i++)
    {
		for (int j = 0; j < map.cells[0].size(); j++)
        {
			if (map.cells[j][i].isObstacle)
				cerr << "X";
			else
				cerr << "_";
		}

		cerr << endl;
	}
}

void printAgents(Map map)
{
	for (int i = 0; i < map.agents.size(); i++)
    {
		cerr << map.agents[i].start.y;
	}
}

void testLowLevel(Map map)
{
	LowLevelSolver solver;
	auto x = solver.findOptimalPaths(vector<Constraint>(), map);

	cerr << x.size();
	for (int i = 0; i < x.size(); i++)
    {
		for (int j = 0; j < x[0].size(); j++)
        {
			cerr << x[i][j].x << "," << x[i][j].y << endl;
		}
	}

}

unsigned int Factorial(unsigned int number)
{
	return number <= 1 ? number : Factorial(number - 1)*number;
}

void print_solution(vector<vector<Cell>> optimalPaths)
{
	for (auto path : optimalPaths)
    {
		cerr << "Optimal path of agent \n";
		for (auto cell : path)
        {
			cerr << cell.x << cell.y << "\n";
		}
	}
}


int main()
{
    auto started = chrono::high_resolution_clock::now();

	vector<vector<Cell>> optimal_paths;
	Map map = read_map("./data/map2.txt");
    cerr << "map:" << endl;
	print_map(map);
	HighLevelSolver solver;
	optimal_paths = solver.solve(map);
	print_solution(optimal_paths);

    auto done = chrono::high_resolution_clock::now();
    auto elapsedTime = chrono::duration_cast<chrono::milliseconds>(done-started).count();

    cerr << elapsedTime << " milliseconds " ;

    return 0;
}

