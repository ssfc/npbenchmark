#ifndef LK_MATRIX
#define LK_MATRIX

#include <vector>

using namespace std;

class LKMatrix {
public:
	int size;
	LKMatrix(vector<pair<float, float> >& coords, vector<int>& ids);
	vector<int> CLK(vector<int> tour, vector<vector<int>> distance_matrix, int kmax);
	float getCurrentTourDistance();
	void optimizeTour();
	void printTour();
	vector<int> getTourIds();
	vector<int> tour;
	vector<vector<int> > edgeFlags;
	vector<pair<float, float> > coords;
	vector<int> ids;
	vector<vector<float> > edgeDistances;
	void LKMove(int tourStart);
	void reverse(int start, int end);
	bool isTour();
};

#endif
