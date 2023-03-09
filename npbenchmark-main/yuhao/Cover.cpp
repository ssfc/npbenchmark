#include "../.h/Cover.h"

Cover::Cover()
{
    this->_size = 0;
    this->_positions = nullptr;
    this->_x = nullptr;
}

Cover::~Cover()
{
    delete[] this->_positions;
    this->_positions = nullptr;
    delete[] this->_x;
    this->_x = nullptr;
}

void Cover::init(int& nodeNum, int& capacity)
{
    this->_positions = new int[nodeNum];
    // for(int i = 0;i < nodeNum;i++)
    // 	this->_positions[i] = -1;
    this->_x = new int[capacity];
}

int Cover::size()
{
    return this->_size;
}

void Cover::clear()
{
    this->_size = 0;
}

void Cover::push_back(const int& v)
{
    this->_x[this->_size] = v;
    this->_positions[v] = this->_size++;
}

void Cover::erase(const int& v)
{
    if(v != this->_x[--this->_size]){
        this->_x[this->_positions[v]] = this->_x[this->_size];
        this->_positions[this->_x[this->_size]] = this->_positions[v];
    }
}

int Cover::operator [](const int& index)
{
    return this->_x[index];
}