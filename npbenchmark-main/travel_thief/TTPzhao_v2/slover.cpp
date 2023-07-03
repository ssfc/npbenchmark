#include"slover.h"
#include <cmath>
#include <ctime>
#include<algorithm>

bool cmp(ItemInfo* a, ItemInfo* b) {
	return a->score > b->score;
}

bool cmpCostPerformance_reverse(ItemInfo* a, ItemInfo* b) {
	return (a->weight / a->profit) > (b->weight / b->profit);
}


bool cmpCostPerformance(ItemInfo * a, ItemInfo * b) {
	return (a->profit / a->weight) > (b->profit / b->weight);
}

void CwsmSlover::initValue(string fileName, string typeFile) {
	ifstream fin;
	fin.open(fileName, ios::in);
	typeFile = "./para_files/" + typeFile;
	if (!fin.is_open())
	{
		cout << "can not find!" << endl;
		return;
	}
	ofstream ofs;
	ofs.open(typeFile + ".par", ios::out);
	if (!ofs.is_open())
	{
		cout << "can not find!" << endl;
		return;
	}
	ofstream ofs_tsp;
	ofs_tsp.open(typeFile + ".tsp", ios::out);
	if (!ofs_tsp.is_open())
	{
		cout << "can not find!" << endl;
		return;
	}

	string buff;
	getline(fin, buff);
	getline(fin, buff);
	getline(fin, buff);
	istringstream tokenStream(buff);
	string str;
	tokenStream >> str;
	tokenStream >> nodeNum;
	ofs << "PROBLEM_FILE = " << typeFile + ".tsp" << endl;
	ofs << "CANDIDATE_SET_TYPE = DELAUNAY" << endl;
	ofs << "SUBGRADIENT = NO" << endl;
	ofs << "IS_ALL_TOURS_RECORD = 1" << endl;
	if (nodeNum < 1000) {
		ofs << "TIME_LIMIT = 30" << endl; //30
	}
	else if (nodeNum >= 1000 && nodeNum <= 3000) {
		ofs << "TIME_LIMIT = 50" << endl; //50
	}
	else {
		ofs << "TIME_LIMIT = 80" << endl; // 80
	}
	ofs << "RUNS = 1" << endl;
	ofs.close();

	int gridNum = 25; // 单元格数量
	gridItems.resize(25);
	left = INT32_MAX;
	right = -1;
	top = -1;
	bottem = INT32_MAX;
	getline(fin, buff);
	istringstream tokenStream_item(buff);
	tokenStream_item >> str; tokenStream_item >> str; tokenStream_item >> str;
	tokenStream_item >> itemNum;

	getline(fin, buff);
	istringstream tokenStream_capacity(buff);
	tokenStream_capacity >> str; tokenStream_capacity >> str; tokenStream_capacity >> str;
	tokenStream_capacity >> capacity;

	getline(fin, buff);
	istringstream tokenStream_minSpeed(buff);
	tokenStream_minSpeed >> str; tokenStream_minSpeed >> str;
	tokenStream_minSpeed >> minspeed;

	getline(fin, buff);
	istringstream tokenStream_maxSpeed(buff);
	tokenStream_maxSpeed >> str; tokenStream_maxSpeed >> str;
	tokenStream_maxSpeed >> maxspeed;

	getline(fin, buff);
	istringstream tokenStream_rent(buff);
	tokenStream_rent >> str; tokenStream_rent >> str;
	tokenStream_rent >> rentrate;


	ofs_tsp << "NAME: " << typeFile << endl;
	ofs_tsp << "COMMENT : city problem(Padberg / Rinaldi)" << endl;
	ofs_tsp << "TYPE : TSP" << endl;
	ofs_tsp << "DIMENSION : " << nodeNum << endl;
	ofs_tsp << "EDGE_WEIGHT_TYPE : EUC_2D" << endl;
	ofs_tsp << "NODE_COORD_SECTION" << endl;


	getline(fin, buff);
	getline(fin, buff);
	// deal coord
	for (int i = 0; i < nodeNum; i++) {
		getline(fin, buff);
		istringstream tokenStream_coord(buff);
		int number;
		tokenStream_coord >> number;
		float x, y;
		tokenStream_coord >> x >> y;
		coord.push_back({ x,y });
		ofs_tsp << i + 1 << "	" << x << "	" << y << endl;
		if (x > right) {
			right = x;
		}
		if (x < left) {
			left = x;
		}

		if (y > top) {
			top = y;
		}
		if (y < bottem) {
			bottem = y;
		}
	}
	ofs_tsp.close();
	int edgeLength = max(right - left, top - bottem);
	if (right - left != edgeLength) {
		right = left + edgeLength;
	}
	else if (top - bottem != edgeLength) {
		top = bottem + edgeLength;
	}
	int gridLength = edgeLength / 5;

	// 看放哪个块上
	int count = 0;
	gridPos.resize(nodeNum, 0);
	for (auto nodePos : coord) {
		int xPos = (nodePos.first - left) / gridLength;
		if (xPos == 5) {
			xPos = 4;
		}
		int yPos = (nodePos.second - bottem) / gridLength;
		if (yPos == 5) {
			yPos = 4;
		}
		int nodeIndex = xPos * 5 + yPos;
		gridItems[nodeIndex].insert(count);
		gridPos[count] = (nodeIndex);
		count++;
	}

	getline(fin, buff);
	citytoitems.assign(nodeNum, vector<int>());
	for (int i = 0; i < itemNum; i++) {
		getline(fin, buff);
		istringstream tokenStream_itemInfos(buff);
		int number;
		tokenStream_itemInfos >> number;
		Item* itemOne = new Item();
		tokenStream_itemInfos >> itemOne->profit >> itemOne->weight >> itemOne->city;
		itemOne->city -= 1;
		itemsAccordIndex.push_back(itemOne);
		citytoitems[itemOne->city].push_back(i);
	}
	velocityConstant = (maxspeed - minspeed) / capacity;
	avergeDistance = calAverDistance();
	fin.close();
}

// 释放申请空间
void CwsmSlover::releaseMemory() {
	for (int i = 0; i < itemNum; i++) {
		delete itemsAccordIndex[i];
	}
}

// 计算背包的目标值
float CwsmSlover::calPackingObj(set<int> & packPlan) {
	float profit = 0;
	for (auto iter = packPlan.begin(); iter != packPlan.end(); iter++) {
		profit += itemsAccordIndex[*iter]->profit;
	}
	return profit;
}



// 计算从某个城市插入或者删除某个item后obj
float CwsmSlover::calObjIncre(vector<int> & tour, set<int> & packPlan, int packObj, int item, bool isErase, int count) {
	float time = 0;
	int weightnow = 0;
	float speed = 0;
	vector<float> cityTime; // 到达此城市已花费时间
	vector<int> cityWeight(weightW[count].begin(), weightW[count].end());
	int city = itemsAccordIndex[item]->city;
	int cityStartIndex = 0; // 进行物品更改的城市
	int weightIncrement = 0; // 重量增量
	if (isErase) {
		weightIncrement = -1 * itemsAccordIndex[item]->weight;
	}
	else {
		weightIncrement = itemsAccordIndex[item]->weight;
	}
	// front cities
	for (int i = 0; i < nodeNum; i++) {
		cityTime.push_back(edgeTimeT[count][i]);
		if (tour[i] == city) {
			time = edgeTimeT[count][i];
			cityStartIndex = i; // 更改城市
			weightnow = weightW[count][i];
			break;
		}
	}
	for (int i = cityStartIndex + 1; i < cityWeight.size(); i++) {
		cityWeight[i] += weightIncrement;
	}


	int testWeight = 0;
	for (int i = 0; i < tour.size(); i++) {
		int visitcity = tour[i];
		for (int j = 0; j < citytoitems[visitcity].size(); j++) {
			int itemNumber = citytoitems[visitcity][j];
			if (packPlan.count(itemNumber) != 0) {
				testWeight += itemsAccordIndex[itemNumber]->weight;
			}
		}
	}

	/*	cout << "test capacity1 " << endl;
		int testWeight = 0;
		for (int i = 0; i < tour.size(); i++) {
			if (tour[i] == city) {
				break;
			}
			int visitcity = tour[i];
			for (int j = 0; j < citytoitems[visitcity].size(); j++) {
				int itemNumber = citytoitems[visitcity][j];
				if (packPlan.count(itemNumber) != 0) {
					testWeight += itemsAccordIndex[itemNumber]->weight;
				}
			}
		}
		cout << "test weight1 " << testWeight << "  real weight1 " << weightnow << endl;

	*/
	// after cities
//	int weightAllTake= weightnow; // 到达该城市时重量
	for (int i = cityStartIndex; i < nodeNum; i++) {
		int visitCity = tour[i];
		speed = maxspeed - (cityWeight[i + 1] * velocityConstant);
		if (i == nodeNum - 1) {
			///			time += edges[tour[i]][tour[0]] / speed;
			time += getNodeDistance(tour[i], tour[0]) / speed;
		}
		else {
			///			time += edges[tour[i]][tour[i + 1]] / speed;
			time += getNodeDistance(tour[i], tour[i + 1]) / speed;
		}
		cityTime.push_back(time);
	}




	float profit;
	if (isErase) {
		profit = packObj - itemsAccordIndex[item]->profit;
	}
	else {
		profit = packObj + itemsAccordIndex[item]->profit;
	}
	//	objSaved[make_pair(tour, packPlan)] = make_pair(time, profit);



	edgeTimeT[count].clear();
	weightW[count].clear();
	edgeTimeT[count].assign(cityTime.begin(), cityTime.end());
	weightW[count].assign(cityWeight.begin(), cityWeight.end());
	float obj = profit - rentrate * time;
	return obj;
}


// 计算目标值
float CwsmSlover::calObj1(vector<int> & tour, set<int> & packPlan) {
	/*	if (objSaved.find(make_pair(tour, packPlan)) != objSaved.end()) {
			auto iter = objSaved.find(make_pair(tour, packPlan));
			return (iter->second.second) -  rentrate * (iter->second.first);
		}
	*/
	float profit = 0;
	float time = 0;
	int weightnow = 0;
	float speed = 0;
	for (int i = 0; i < nodeNum; i++) {
		int visitcity = tour[i];
		for (int j = 0; j < citytoitems[visitcity].size(); j++) {
			int itemNumber = citytoitems[visitcity][j];
			if (packPlan.count(itemNumber) != 0) {
				weightnow += itemsAccordIndex[itemNumber]->weight;
				profit += itemsAccordIndex[itemNumber]->profit;
			}
		}
		speed = maxspeed - (weightnow * velocityConstant);
		if (i == nodeNum - 1) {
			//			time += edges[tour[i]][tour[0]] / speed;
			time += getNodeDistance(tour[0], tour[i]) / speed;
		}
		else {
			//			time += edges[tour[i]][tour[i + 1]] / speed;
			time += getNodeDistance(tour[i], tour[i + 1]) / speed;
		}
	}
	//	objSaved[make_pair(tour, packPlan)] = make_pair(time, profit);
	float obj = profit - rentrate * time;
	return obj;
}

// 计算目标值
float CwsmSlover::calObj(vector<int> & tour, set<int> & packPlan) {
	/*	if (objSaved.find(make_pair(tour, packPlan)) != objSaved.end()) {
			auto iter = objSaved.find(make_pair(tour, packPlan));
			return (iter->second.second) -  rentrate * (iter->second.first);
		}
	*/
	float profit = 0;
	float time = 0;
	int weightnow = 0;
	float speed = 0;
	for (int i = 0; i < nodeNum; i++) {
		int visitcity = tour[i];
		for (int j = 0; j < citytoitems[visitcity].size(); j++) {
			int itemNumber = citytoitems[visitcity][j];
			if (packPlan.count(itemNumber) != 0) {
				weightnow += itemsAccordIndex[itemNumber]->weight;
				profit += itemsAccordIndex[itemNumber]->profit;
			}
		}
		speed = maxspeed - (weightnow * velocityConstant);
		if (i == nodeNum - 1) {
			//			time += edges[tour[i]][tour[0]] / speed;
			time += getNodeDistance(tour[0], tour[i]) / speed;
		}
		else {
			//			time += edges[tour[i]][tour[i + 1]] / speed;
			time += getNodeDistance(tour[i], tour[i + 1]) / speed;
		}
	}
	//	objSaved[make_pair(tour, packPlan)] = make_pair(time, profit);
	float obj = profit - rentrate * time;
	return obj;
}

// 单独计算每个的目标值
// 计算目标值
void CwsmSlover::calSoleObj(vector<int> & tour, set<int> & packPlan, float& tourObj, float& packingObj) {
	tourObj = 0;
	packingObj = 0;
	int weightnow = 0;
	float speed = 0;
	for (int i = 0; i < nodeNum; i++) {
		int visitcity = tour[i];
		for (int j = 0; j < citytoitems[visitcity].size(); i++) {
			if (packPlan.count(citytoitems[visitcity][j]) != 0) {
				weightnow += itemsAccordIndex[citytoitems[visitcity][j]]->weight;
				packingObj += itemsAccordIndex[citytoitems[visitcity][j]]->profit;
				speed = maxspeed - (weightnow / capacity) * (maxspeed - minspeed);
				if (i == nodeNum - 1) {
					tourObj += edges[tour[i]][tour[0]] / speed;
				}
				else {
					tourObj += edges[tour[i]][tour[i + 1]] / speed;
				}
			}
		}
	}
	return;
}

// 根据公式计算得分
void CwsmSlover::calItemScore(float a, vector<int> & tour) {
	vector<float> pathSumToEnd(nodeNum, 0); // 路径上每点到终点距离
	for (int i = nodeNum - 2; i >= 0; i--) {
		//		pathSumToEnd[tour[i]] = pathSumToEnd[tour[i + 1]] + edges[tour[i]][tour[i + 1]];
		pathSumToEnd[tour[i]] = pathSumToEnd[tour[i + 1]] + getNodeDistance(tour[i], tour[i + 1]);
	}
	pathSumToEnd[tour[nodeNum - 1]] = 0.001; // 防止除数为0
	for (int i = 0; i < itemNum; i++) {
		int city = itemsAccordIndex[i]->city;
		itemsAccordIndex[i]->score = pow(itemsAccordIndex[i]->profit, a) / ((pow(itemsAccordIndex[i]->weight, a)) * pathSumToEnd[city]);
	}
	return;
}

// 随机装包算法 Rho, objPara, Gamma, objSaved
set<int> CwsmSlover::randomPacking(vector<int> & tour, int Rho, int Gamma) {
	set<int> bestPacking;
	for (int i = 0; i < Rho; i++) {
		// calcute items scores and sort
		float a = (float)(rand() % 10);
		/*		float b = (float)(rand() % 10) / 1000;144
				float c = (float)(rand() % 10) / 1000;
				float sum = a + b + c;
				a = a / sum;
				b = b / sum;
				c = c / sum;
		*/
		calItemScore(a, tour);
		vector<ItemInfo*> items;
		for (int j = 0; j < itemNum; j++) {
			ItemInfo* itemOne = new ItemInfo(j, itemsAccordIndex[j]->weight, itemsAccordIndex[j]->profit, itemsAccordIndex[j]->city, itemsAccordIndex[j]->score);
			items.push_back(itemOne);
		}
		sort(items.begin(), items.end(), cmp);
		// generate packings
		// 计算目标次数
		int frequency = itemNum / 50;
		if (itemNum >= 169044 && itemNum <= 200000) {
			frequency = itemNum / 15;
		}
		else if (itemNum > 200000) {
			frequency = itemNum / 2;
		}
		set<int> packOne;
		set<int> packTwo;
		bool newPackingPlan = false;
		int k = 1, k_ = 1;

		int bagOccupy = 0; // 已占用包重量
		while (k_ < itemNum && frequency >= 1) {
			if (bagOccupy + items[k_]->weight <= capacity) {
				bagOccupy += items[k_]->weight;
				packTwo.insert(items[k_]->number);
				newPackingPlan = true;
			}
			if (k_ % frequency == 0 && newPackingPlan) {
				if (calObj(tour, packTwo) > calObj(tour, packOne)) {
					packOne = packTwo;
					k = k_;
				}
				else {
					packTwo = packOne;
					k_ = k;
					frequency = frequency / 2;
					newPackingPlan = false;
				}
			}
			k_ += 1;
		}
		if (calObj(tour, packOne) > calObj(tour, bestPacking)) {
			bestPacking = packOne;
		}
		for (int j = 0; j < itemNum; j++) {

			delete items[j];
		}
	}
	return bestPacking;
}


// 点太多时空间不足，计算两点之间距离
float CwsmSlover::getNodeDistance(int nodeA, int nodeB) {
	return ceil(sqrt(pow((coord[nodeA].first - coord[nodeB].first), 2) +
		pow((coord[nodeA].second - coord[nodeB].second), 2)));
}

// 求城市间的平均距离
float CwsmSlover::getDistance() {
	float sum = 0;
	for (int i = 0; i < edges.size(); i++) {
		for (int j = 0; j < i; j++) {
			sum += edges[i][j];
		}
	}
	float aver = sum / edges.size();
	return aver;
}

// 获得当前obj最好的解
pair<vector<int>, set<int>> CwsmSlover::getBestSoluS(map<pair<vector<int>, set<int>>, pair<float, float>> & objSaved, set<pair<vector<int>, set<int>>> & S) {
	float obj = 0;
	auto bestObjIter = S.begin();
	for (auto iter = S.begin(); iter != S.end(); iter++) {
		float temObj = objSaved[*iter].second - rentrate * objSaved[*iter].first;
		if (temObj > obj) {
			obj = temObj;
			bestObjIter = iter;
		}
	}
	return *bestObjIter;
}

// 2-opt 获得所有路径更短的集合
void CwsmSlover::twoOpt(vector<int> & tour, set<vector<int>> & toursSet) {
	bool improved = true;
	// Repeat until no further improvements can be made.
	while (improved) {
		// Set the improved flag to false.
		improved = false;

		// Consider each locations in the route.
		for (int i = 0; i < tour.size(); i++) {
			for (int j = i + 2; j < tour.size(); j++) {
				// Calculate the distances between the locations in the current route.
				double d1 = getNodeDistance(tour[i], tour[i + 1]);
				double d2 = getNodeDistance(tour[j], tour[(j + 1) % tour.size()]);
				double d3 = getNodeDistance(tour[i], tour[j]);
				double d4 = getNodeDistance(tour[i + 1], tour[(j + 1) % tour.size()]);

				// If swapping the order of the locations would result in a shorter
				// route, reverse the locations
				if (d1 + d2 > d3 + d4) {
					vector<int> twoOptTour(tour.begin(), tour.end());
					reverse(twoOptTour.begin() + i + 1, twoOptTour.begin() + j + 1);
					toursSet.insert(twoOptTour);
					improved = true;
					tour.assign(twoOptTour.begin(), twoOptTour.end());
				}
			}
		}
	}

	// Return the improved route.
	return;
}

// 随机插入获得路径
void CwsmSlover::randomInsertTour(vector<int> & tour, set<vector<int>> & toursSet) {
	bool improved = true;
	// Repeat until no further improvements can be made.
	int iter = 0;
	while (iter < 5) {
		// Set the improved flag to false.
		int randA = rand() % tour.size();
		int randB = rand() % tour.size(); // 把randA插入到randB
		vector<int> temp_tour(tour.size());
		if (randA < randB) {
			for (int i = 0; i < randA; i++) {
				temp_tour.push_back(tour[i]);
			}
			int nodeA = tour[randA];
			for (int i = randA + 1; i < randB; i++) {
				temp_tour.push_back(tour[i]);
			}
			temp_tour.push_back(nodeA);
			for (int i = randB; i < tour.size(); i++) {
				temp_tour.push_back(tour[i]);
			}
		}
		else if (randA > randB) {
			int nodeA = tour[randA];
			for (int i = 0; i < randB; i++) {
				temp_tour.push_back(tour[i]);
			}
			temp_tour.push_back(nodeA);
			for (int i = randB; i < randA; i++) {
				temp_tour.push_back(tour[i]);
			}
			for (int i = randA + 1; i < tour.size(); i++) {
				temp_tour.push_back(tour[i]);
			}
		}
		toursSet.insert(temp_tour);
	}

	// Return the improved route.
	return;
}


// 计算tour路径长度(回到原点)
float CwsmSlover::calTourDistance(vector<int> & tour) {
	float sum = 0;
	for (int i = 0; i < tour.size() - 1; i++) {
		sum += getNodeDistance(tour[i], tour[i + 1]);
	}
	sum += getNodeDistance(tour[tour.size() - 1], tour[0]);
	return sum;
}

// 计算平均距离
float CwsmSlover::calAverDistance() {
	float sum = 0;
	float edgeDistance;
	int edgeCount = 0;
	for (int i = 0; i < nodeNum; i++) {
		for (int j = 0; j < i; j++) {
			// Compute the edge distance
			edgeDistance = sqrt(pow((coord[i].first - coord[j].first), 2) +
				pow((coord[i].second - coord[j].second), 2));
			sum += edgeDistance;
			edgeCount++;
		}
	}
	return sum / edgeCount;
}

// 计算每两个城市之间的距离
void CwsmSlover::calDistance() {
	edges.resize(nodeNum);
	for (int i = 0; i < nodeNum; i++) {
		edges[i].resize(nodeNum, 0);
	}
	float edgeDistance;
	for (int i = 0; i < nodeNum; i++) {
		for (int j = 0; j <= i; j++) {
			// Compute the edge distance
			edgeDistance = sqrt(pow((coord[i].first - coord[j].first), 2) +
				pow((coord[i].second - coord[j].second), 2));
			edges[i][j] = edgeDistance;
			edges[j][i] = edgeDistance;
		}
	}
}

// 初始化tour中边到达之间的权重和时间
void CwsmSlover::initEdgeTime(vector<int> & tour, set<int> & packing, int count) {
	float profit = 0;
	float time = 0;
	int weightnow = 0;
	float speed = 0;
	edgeTimeT[count].clear();
	weightW[count].clear();
	edgeTimeT[count].push_back(0);
	weightW[count].push_back(0);
	for (int i = 0; i < nodeNum; i++) {
		int visitcity = tour[i];
		for (int j = 0; j < citytoitems[visitcity].size(); j++) {
			int itemNumber = citytoitems[visitcity][j];
			if (packing.count(itemNumber) != 0) {
				weightnow += itemsAccordIndex[itemNumber]->weight;
				profit += itemsAccordIndex[itemNumber]->profit;
			}
		}
		weightW[count].push_back(weightnow);
		speed = maxspeed - (weightnow * velocityConstant);
		if (i == nodeNum - 1) {
			//getNodeDistance(tour[i], tour[i + 1]);
//			time += edges[bestTour[i]][bestTour[0]] / speed;
			time += getNodeDistance(tour[i], tour[0]) / speed;
			edgeTimeT[count].push_back(time);
		}
		else {
			//			time += edges[bestTour[i]][bestTour[i + 1]] / speed;
			time += getNodeDistance(tour[i], tour[i + 1]) / speed;
			edgeTimeT[count].push_back(time);
		}
	}
	return;
}

// 计算besttour&bestpacking:items in city
void CwsmSlover::bestTourPackings() {
	besttour_citytoitems.clear();
	set<int> citypackingCity0; // 这个不是装所有的，装一部分 可以算一下上限和下限
	besttour_citytoitems.push_back(citypackingCity0);
	for (int i = 0; i < nodeNum; i++) {
		for (int j = 0; j < citytoitems[bestTour[i]].size(); j++) {
			set<int> citypacking; // 这个不是装所有的，装一部分 可以算一下上限和下限
			if (bestPacking.count(citytoitems[bestTour[i]][j]) != 0) {
				citypacking.insert(citytoitems[bestTour[i]][j]);
			}
			besttour_citytoitems.push_back(citypacking);
		}
	}
}

// Iterative Packing Routine PackIterative (Π,c,δ,q)
set<int> CwsmSlover::iterativePacking(vector<int> & tour, double c, double delta, int q) {
	double ep = 1;
	set<int> p_l, p_m, p_r;
	double z_l, z_r, z_m;
	pair<set<int>, double> T;
	T = routinePack(tour, c); p_m = T.first; z_m = T.second;
	T = routinePack(tour, c - delta); p_l = T.first; z_l = T.second;
	T = routinePack(tour, c + delta); p_r = T.first; z_r = T.second;
	set<int> bestPacking;
	for (int i = 0; i < q; i++) {
		if (z_l > z_m && z_r > z_m) {
			if (z_l > z_r) {
				z_m = z_l;
				c = c - delta;
				bestPacking = p_l;
			}
			else {
				z_m = z_r;
				c = c + delta;
				bestPacking = p_r;
			}
		}
		else if (z_l > z_m) {
			z_m = z_l;
			c = c - delta;
			bestPacking = p_l;
		}
		else if (z_r > z_m) {
			z_m = z_r;
			c = c + delta;
			bestPacking = p_r;
		}
		delta = delta / 2;
		T = routinePack(tour, c - delta); p_l = T.first; z_l = T.second;
		T = routinePack(tour, c + delta); p_r = T.first; z_r = T.second;
		if (abs(z_l - z_m) < ep && abs(z_r - z_m) < ep) break;
	}
	return bestPacking;
}

pair<set<int>, double> CwsmSlover::routinePack(vector<int> & tour, double a) {
	double best_f = 0;
	set<int> bestPacking;
	// calcute items scores and sort
//        default_random_engine e;
//        uniform_real_distribution<double> u(1,6);
//        double a = u(e);
	calItemScore(a, tour);
	vector<ItemInfo*> items;
	for (int j = 0; j < itemNum; j++) {
		ItemInfo* itemOne = new ItemInfo(j, itemsAccordIndex[j]->weight, itemsAccordIndex[j]->profit, itemsAccordIndex[j]->city, itemsAccordIndex[j]->score);
		items.push_back(itemOne);
	}
	sort(items.begin(), items.end(), cmp);
	// generate packings

	int frequency = itemNum / 30;
	if (itemNum >= 169044 && itemNum <= 200000) {
		frequency = itemNum / 15;
	}
	else if (itemNum > 200000) {
		frequency = itemNum / 10;
	}

	set<int> packOne;
	set<int> packTwo;
	bool newPackingPlan = false;
	int k = 1, k_ = 1;

	int bagOccupy = 0; // 已占用包重量
	while (k_ < itemNum && frequency >= 1) {
		if (bagOccupy + items[k_]->weight <= capacity) {
			bagOccupy += items[k_]->weight;
			packTwo.insert(items[k_]->number);
			newPackingPlan = true;
		}
		if (k_ % frequency == 0 && newPackingPlan) {
			if (calObj(tour, packTwo) > calObj(tour, packOne)) {
				packOne = packTwo;
				k = k_;
			}
			else {
				packTwo = packOne;
				k_ = k;
				frequency = frequency / 2;
				newPackingPlan = false;
			}
		}
		k_ += 1;
	}
	double f1 = calObj(tour, packOne);
	double f2 = calObj(tour, bestPacking);
	if (f1 > f2) {
		bestPacking = packOne;
		best_f = f1;
	}
	for (int j = 0; j < itemNum; j++) {
		delete items[j];
	}
	return { bestPacking, best_f };
}


void CwsmSlover::readTours(vector<vector<int>> & initTours, string path) {
	ifstream  fin;
	fin.open(path, ios::in);
	if (!fin.is_open())
	{
		cerr << "cannot open the file" << endl;
	}
	string line;
	while (getline(fin, line)) {
		istringstream ss(line);
		int tmp;
		vector<int> tempTour;
		while (ss >> tmp)
		{
			tempTour.push_back(tmp);
		}
		initTours.push_back(tempTour);
	}
	fin.close();
}

set<int> CwsmSlover::initPackings(vector<int> & tour, int distancePara, int influenceTimesPara) {
	set<int> bestPacking;
	// calcute items scores and sort
	float a = (float)(rand() % 10);
	/*		float b = (float)(rand() % 10) / 1000;144
			float c = (float)(rand() % 10) / 1000;
			float sum = a + b + c;
			a = a / sum;
			b = b / sum;
			c = c / sum;
	*/
	calItemScore(a, tour);
	vector<ItemInfo*> items;
	for (int j = 0; j < itemNum; j++) {
		ItemInfo* itemOne = new ItemInfo(j, itemsAccordIndex[j]->weight, itemsAccordIndex[j]->profit, itemsAccordIndex[j]->city, itemsAccordIndex[j]->score);
		items.push_back(itemOne);
	}
	sort(items.begin(), items.end(), cmp);
	// generate packings
	set<int> packOne;
	bool newPackingPlan = false;
	int k = 1, k_ = 1;

	int bagOccupy = 0; // 已占用包重量
	while (k_ < itemNum && bagOccupy < capacity) {
		if (bagOccupy + items[k_]->weight <= capacity) {
			bagOccupy += items[k_]->weight;
			packOne.insert(items[k_]->number);
		}
		else {
			break;
		}
		k_++;
	}
	for (int j = 0; j < itemNum; j++) {
		delete items[j];
	}
	return packOne;
}

// 计算背包中物品重量
float CwsmSlover::calPackWeight(set<int> packing) {
	float sum = 0;
	for (auto iter = packing.begin(); iter != packing.end(); iter++) {
		sum += itemsAccordIndex[*iter]->weight;
	}
	return sum;
}

// wsm算法
float CwsmSlover::weightedSum(int Eta, int Rho, int Gamma, int Beta, int Lambda, char* paraFile, string resultPath, clock_t start) {
	float duration;
	time_t timep;
	time(&timep);
	string fileName;
	cout.setf(ios::fixed, ios::floatfield);
	cout.precision(2);
	bestValue = 0;
	clock_t finish;
	vector<vector<int>> initTours;
	int* tourArray = LKH(paraFile, initTours);
	//    initTours.push_back(tour_temp);
	reverse(initTours.begin(), initTours.end());
	int resavedSize = 15;
	if (itemNum >= 169044 && itemNum <= 200000) {
		resavedSize = 12;
	}
	else if (itemNum > 200000) {
		resavedSize = 3;
	}
	set<vector<int>> reapInitTours(initTours.begin(), initTours.end());
	initTours.clear();
	initTours.assign(reapInitTours.begin(), reapInitTours.end()); // 去除重复tour
	resavedSize = resavedSize > initTours.size() * 2 ? initTours.size() * 2 : resavedSize;
	int realSize = initTours.size();

	// Convert to 1 to start
	for (int i = 0; i < realSize; i++) {
		vector<int> tour_temp(initTours[i].begin(), initTours[i].end());
		for (int j = 0; j < nodeNum; j++) {
			tour_temp[j] -= 1;
		}
		int index = distance(tour_temp.begin(), find(tour_temp.begin(), tour_temp.end(), 0));
		int count = 0;
		for (int j = index; j < tour_temp.size(); j++) {
			initTours[i][count] = tour_temp[j];
			count++;
		}
		for (int j = 0; j < index; j++) {
			initTours[i][count] = tour_temp[j];
			count++;
		}
	}

	// best tour : Convert to 1 to start
   // test reverse
	set<vector<int>> reapInitTours_reverse(initTours.begin(), initTours.end());
	for (auto tour_in_set : reapInitTours_reverse) {
		reverse((tour_in_set.begin() + 1), tour_in_set.end());
		initTours.push_back(tour_in_set);
	}

	vector<set<int>> tourPackings(resavedSize, set<int>()); // 初始tour对应的局部搜索
	map<pair<vector<int>, set<int>>, float> temporarilyObj;

	// init packing value
	for (int tourIndex = 0; tourIndex < resavedSize; tourIndex++) {
		tourPackings[tourIndex] = iterativePacking(initTours[tourIndex], 5, 2.5, 20);
		float obj = calObj(initTours[tourIndex], tourPackings[tourIndex]);
		if (obj > bestValue) {
			bestValue = obj;
			bestPacking = tourPackings[tourIndex];
			bestTour = initTours[tourIndex];
		}
		temporarilyObj[{initTours[tourIndex], tourPackings[tourIndex]}] = obj;  // 36,000
		finish = clock();
		duration = (double)(finish - start) / CLOCKS_PER_SEC;
		if (duration >= 593) {
			fileName = resultPath + "REVAlgorithm" + to_string(time(0));
			writeResult(fileName);
			return bestValue;
		}
	}

	// change tour ： insert




	for (int packingIter = 0; packingIter < resavedSize; packingIter++) {
		int iterTime = 20;
		int Gai = 50;
		if (itemNum >= 4460) {
			iterTime = 2;
		}
		for (int bitIter = 0; bitIter < iterTime; bitIter++) {
			if (tourPackings[packingIter].size() == 0) {
				continue;
			}
			set<int> randomPackingTemp(tourPackings[packingIter].begin(), tourPackings[packingIter].end()); // 全都是在randomPackingTemp上修改 第一次迭代后obj变好可以更改

			// 先删除几个性价比低的 然后再去插入
			vector<ItemInfo*> items;
			for (int j = 0; j < randomPackingTemp.size(); j++) {
				ItemInfo* itemOne = new ItemInfo(j, itemsAccordIndex[j]->weight, itemsAccordIndex[j]->profit, itemsAccordIndex[j]->city, itemsAccordIndex[j]->score);
				items.push_back(itemOne);
			}
			sort(items.begin(), items.end(), cmpCostPerformance_reverse);
			int eraseNum = 0;
			if (randomPackingTemp.size() <= 50) {
				eraseNum = 4;
			}
			else if (randomPackingTemp.size() <= 500) {
				eraseNum = 10;
			}
			else {
				eraseNum = 15;
			}
			for (int i = 0; i < eraseNum; i++) {
				randomPackingTemp.erase(items[i]->number);
			}

			for (int i = nodeNum - 1; i > 0; i--) {
				for (int j = 0; j < citytoitems[initTours[packingIter][i]].size(); j++) {
					int alterItemIndex = citytoitems[initTours[packingIter][i]][j];
					if ((rand() % 100) < Gai) {  // 优先挑 挑性价比低 但是重的删除 删除的时候需要仔细观察 随机不应该大概率随机 小概率去随机
						if (randomPackingTemp.find(alterItemIndex) == randomPackingTemp.end() && calPackWeight(randomPackingTemp) + itemsAccordIndex[alterItemIndex]->weight <= capacity) {
							randomPackingTemp.insert(alterItemIndex);
							float obj = calObj(initTours[packingIter], randomPackingTemp);
							if (obj > bestValue) {
								bestPacking = randomPackingTemp;
								bestTour = initTours[packingIter];
								tourPackings[packingIter] = randomPackingTemp;
								bestValue = obj;
							}
						}
					}
 				finish = clock();
					if ((double)(finish - start) / CLOCKS_PER_SEC > 595) {
						fileName = resultPath + "REVAlgorithm" + to_string(time(NULL));
						writeResult(fileName);
						return bestValue;
					}

				}

			}
			for (int i = 0; i < items.size(); i++) {
				delete items[i];
			}
			finish = clock();
			if ((double)(finish - start) / CLOCKS_PER_SEC > 595) {
				fileName = resultPath + "REVAlgorithm" + to_string(time(NULL));
				writeResult(fileName);
				return bestValue;
			}
		}
	}

	fileName = resultPath + "REVAlgorithm" + to_string(time(NULL));
	writeResult(fileName);
	return bestValue;
}

void CwsmSlover::writeResult(string solverFile) {
	ofstream outFile;
	outFile.open(solverFile, ios::out); // 打开模式可省略
	if (!outFile.is_open())
	{
		std::cout << "Error: opening file fail" << std::endl;
		std::exit(1);
	}
	outFile << "[";
	for (int i = 0; i < bestTour.size()-1; i++) {
		outFile << bestTour[i] + 1<<"," ;
	}
	outFile << bestTour[bestTour.size() - 1] + 1 ;
	outFile << "]";
	outFile << endl;
	auto tem = --bestPacking.end();
	outFile << "[";
	for (auto it = bestPacking.begin(); it != tem;it++) {
		outFile << (*it)+1<<"," ;
	}
	outFile << (*tem) + 1;
	outFile << "]";
	outFile.close();
	return;
}


bool CwsmSlover::testPackingCapacity() {
	int weight = 0;
	for (auto item : bestPacking) {
		weight += itemsAccordIndex[item]->weight;
	}
	if (weight > capacity) {
		return false;
	}
	cout << "weight: " << weight << " capacity: " << capacity << endl;
	return true;
}

float CwsmSlover::testCalObj(vector<int> & tour, set<int> & packing) {
	float profit = 0;
	float time = 0;
	int weightnow = 0;
	float speed = 0;
	for (int i = 0; i < nodeNum; i++) {
		int visitcity = tour[i];
		for (int j = 0; j < citytoitems[visitcity].size(); j++) {
			int itemNumber = citytoitems[visitcity][j];
			if (packing.count(itemNumber) != 0) {
				weightnow += itemsAccordIndex[itemNumber]->weight;
				profit += itemsAccordIndex[itemNumber]->profit;
			}
		}
		speed = maxspeed - (weightnow * velocityConstant);
		if (i == nodeNum - 1) {
			time += edges[tour[i]][tour[0]] / speed;
		}
		else {
			time += edges[tour[i]][tour[i + 1]] / speed;
		}
	}
	return  profit - rentrate * time;
}
