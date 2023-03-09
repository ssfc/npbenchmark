#include "../.h/PCenter.h"
#include "../.h/Cover.h"

PCenter::PCenter()
{
	this->nodeNum = 0;
	this->centerNum = 0;
	this->Xindex = 0;
	this->dropLen = 0;
	this->w = nullptr;
	this->Delta = nullptr;
	this->positions = nullptr;
	this->sizes = nullptr;
	this->tabulist = nullptr;
	this->covers = nullptr;
	this->dropsSizes = nullptr;
	this->coverages = nullptr;
	this->nodesWithDrops = nullptr;
	this->X = nullptr;
}

void PCenter::init()
{
	this->w = new int[this->nodeNum];
	this->Delta = new int[this->nodeNum];
	this->coverages = new int*[this->nodeNum];
	this->positions = new int[this->nodeNum];
	this->covers	= new Cover[this->nodeNum];
	this->sizes		= new int[this->nodeNum];
	this->tabulist	= new int[this->nodeNum];
	this->X			= new int[this->centerNum];
}

void PCenter::init(const int& maxEdge, const int& minEdge)
{
	this->dropLen = maxEdge - minEdge;
	this->nodesWithDrops = new int*[this->dropLen];
	this->dropsSizes = new int[this->dropLen];
}

PCenter::~PCenter()
{
	int i;
	for(i = 0;i < this->nodeNum;i++)
		delete[] this->coverages[i];
	delete[] this->coverages;
	delete[] this->positions;
	delete[] this->covers;
	delete[] this->sizes;
	delete[] this->tabulist;
	delete[] this->X;
	for(i = 0;i < this->dropLen;i++)
		delete[] this->nodesWithDrops[i];
	delete[] this->nodesWithDrops;
	delete[] this->dropsSizes;
}