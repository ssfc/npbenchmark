#ifndef SLOVER
#define SLOVER

#include<vector>
#include<set>
#include<algorithm>
#include<map>
#include<fstream>
#include <iostream>
#include<string>
#include<sstream>
#include <numeric>
#include<time.h>
#include"LKH.h"
#include"LKMatrix.h"
using namespace std;

#define E 1.0e-5

struct ItemInfo {
	int number; // item���
	int weight;
	int profit;
	int city; // ��Ʒ���ڳ���
	float score; // ��Ʒ�÷�
	ItemInfo(int itemNumber, int itemWeight, int itemProfit, int itemCity, float itemScore) {
		number = itemNumber;
		weight = itemWeight;
		profit = itemProfit;
		city = itemCity;
		score = itemScore;
	}
	ItemInfo(int itemNumber, int itemWeight, int itemProfit, int itemCity) {
		number = itemNumber;
		weight = itemWeight;
		profit = itemProfit;
		city = itemCity;
		score = 0;
	}
};
struct Item {
	int weight;
	int profit;
	int city;
	float score; // ���濴�ǲ���û��Ҫ
};

class CwsmSlover {
public:
	int itemNum;
	int nodeNum;
	int capacity;
	float minspeed;
	float maxspeed;
	float rentrate;
	float avergeDistance;
	float bestValue;
	float velocityConstant;
	int left;
	int right;
	int top;
	int bottem;

	vector<pair<float, float>> coord;
	vector<int> gridPos; // �����ڿ�λ��
	vector<Item*> itemsAccordIndex;
	vector<vector<float>> edges; // ÿ������֮��ľ���
	vector<vector<int>> citytoitems; // ÿ�����ж�Ӧ����Ʒ
	vector<set<int>> besttour_citytoitems; // bestTourÿ������ѡ����Ʒ
	vector<int> itemToCity;
	map<vector<int>, float> pathLength; // ·������
	vector<vector<float>> edgeTimeT;
	vector<vector<int>> weightW; // ��ֹ���������Ϊֹ������
	vector<int> bestTour;
	set<int> bestPacking;
	vector<set<int>> gridItems;

	set<int> randomPacking(vector<int>& tour, int distancePara, int influenceTimesPara);
	float weightedSum(int Eta, int Rho, int Gamma, int Beta, int Lambda,char* paraFile, string resultPath, clock_t start);
	void calItemScore(float a, vector<int>& tour);
	void twoOpt(vector<int>& tour, set<vector<int>>& toursSet);
	float getDistance();
	float calTourDistance(vector<int>& tour);
	float calPackWeight(set<int> packing);
	void calDistance();
	bool testPackingCapacity();
	float testCalObj(vector<int>& tour, set<int>& packing);
	float calObjIncre(vector<int>& tour, set<int>& packPlan, int packObj, int item, bool isErase, int count);
	float calPackWeight1(set<int> packing, int item);
	void bestTourPackings();
	float getNodeDistance(int nodeA,int nodeB);
	void readTours(vector<vector<int>>& initTours,string tourpath);
	set<int> iterativePacking(vector<int>& tour, double c, double delta, int q);
	pair<set<int>, double> routinePack(vector<int>& tour, double a);

	float calObj(vector<int>& tour, set<int>& packPlan);
	float calPackingObj(set<int>& packPlan);
	void calSoleObj(vector<int>& tour, set<int>& packPlan, float& tourObj, float& packingObj);
	bool updateS(vector<int>& tour, set<int>& packing, set<pair<vector<int>, set<int>>>& S, float& packingObj, float& tourTime);
	bool updateSoleS(vector<int>& tour, set<int>& packing, set<pair<vector<int>, set<int>>>& S, float& packingObj, float& tourTime);
	pair<vector<int>, set<int>> getBestSoluS(map<pair<vector<int>, set<int>>, pair<float, float>>& objS,set<pair<vector<int>, set<int>>>& S);
	float getBestSoluValue(  set<pair<vector<int>, set<int>>>& S);
	float calAverDistance();
	void randomInsertTour(vector<int>& tour, set<vector<int>>& toursSet);
	set<int> initPackings(vector<int>& tour, int distancePara, int influenceTimesPara);
	float calObj1(vector<int>& tour, set<int>& packPlan);

	void initValue(string fileName,string typeFile);
	void writeResult(string solverFile);
	void initEdgeTime(vector<int>& tour, set<int>& packing, int count);
	void releaseMemory();
};
#endif