#include<iostream>
#include<vector>
#include <fstream>
#include"slover.h"
#include "LKMatrix.h"

int main(int argc, char* argv[]) {
	clock_t start, finish;
	start = clock();
//	ofstream outFile; 
//	outFile.open("result_6_29_1.csv", ios::app); // 打开模式可省略
//	if (!outFile.is_open())
//	{
//		std::cout << "Error: opening file fail" << std::endl;
//		std::exit(1);
//	}
    string fileName = argv[1];
	string pureName= fileName.substr(0, fileName.rfind("."));

	unsigned int seed = time(0);
	srand(seed);
	double  duration;
	string file = "./input_files/"+fileName;
	CwsmSlover slover;


	slover.initValue(file, pureName);
	char paraFile[200];
	string paraFileName = "./para_files/" + pureName + ".par";
//	pureName = pureName + ".par";
	int i = 0;
	for (i = 0; i < paraFileName.size(); i++) {
		paraFile[i] = paraFileName[i];
	}
	paraFile[i] = '\0';
	int randName = rand();
	string resultPath= "./result_files/"+pureName+".ttp.";
	float value = slover.weightedSum(117, 12, 41, 0.001, 22, paraFile, resultPath, start);
	cout << value << endl;
//	finish = clock();
//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
//	outFile << fileName << ',' << duration << ','  << value<<","<< seed << endl;
//	cout << fileName << ',' << duration << ',' << 100 << ',' << value << endl;
//	outFile.close();
	return 0;
}
