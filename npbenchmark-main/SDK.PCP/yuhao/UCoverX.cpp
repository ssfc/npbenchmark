#include <algorithm>

#include "../.h/UCoverX.h"
#include "../.h/PCenter.h"

int UCoverX::capacity = 0;

UCoverX::UCoverX()
{
    this->__size = 0;
    this->ux = nullptr;
}

UCoverX::UCoverX(const UCoverX& UX)
{
    this->__size = UX.__size;
    this->ux = new int[UCoverX::capacity];
    for(int i = 0;i < this->__size;i++)
        this->ux[i] = UX.ux[i];
}

void UCoverX::setCapacity(int __capacity)
{
    UCoverX::capacity = __capacity;
    this->ux = new int[UCoverX::capacity];
}

int UCoverX::size()
{
    return this->__size;
}

void UCoverX::clear()
{
    this->__size = 0;
}

void UCoverX::erase(PCenter& input, const int& index)
{
    if(index < 0)
        return;
    if(this->ux[index] != this->ux[--__size]){
        std::swap(this->ux[index], this->ux[this->__size]);
        input.positions[this->ux[index]] = input.positions[this->ux[this->__size]];
    }
    input.positions[this->ux[this->__size]] = -1;
}

void UCoverX::push_back(PCenter& input, const int& v)
{
    this->ux[this->__size++] = v;
    input.positions[v] = -(this->__size - 1) - 2;
}

UCoverX& UCoverX::operator=(const UCoverX& UX)
{
    if(this == &UX)
        return *this;
    if(this->ux)
        delete this->ux;
    this->__size = UX.__size;
    this->ux = new int[UCoverX::capacity];
    for(int i = 0;i < this->__size;i++)
        this->ux[i] = UX.ux[i];
    return *this;
}

int UCoverX::operator [](const int& index)
{
    return this->ux[index];
}