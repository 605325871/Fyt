#pragma once
#include<iostream>
using namespace std;
class myarr
{
private:
    int * _myarraderss;
    int _capcity;
    int _size;//数组个数
public:
    myarr();//默认构造默认100容量
    myarr(int cpacity);
    myarr(const myarr&arry);
    void push_back(int val);
    int getdata(int index);
    void setdata(int index,int val);
    ~myarr();
};


