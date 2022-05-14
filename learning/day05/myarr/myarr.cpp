#include"myarr.h"
myarr::myarr()
{
    this->_capcity=100;
    this->_size = 0;
    this->_myarraderss = new int[this->_capcity];
}
myarr::myarr(const myarr& arry)
{
    this->_myarraderss= new int[arry._capcity];
    this->_capcity = arry._capcity;
    this->_size=arry._size;

    for(int i=0 ; i<arry._size; ++i)
    {
        this->_myarraderss[i]=arry._myarraderss[i];
    }
}
myarr::myarr(int capcity)
{
    cout<<"有参调用"<<endl;
    this->_capcity=capcity;
    this->_size = 0;
    this->_myarraderss = new int[this->_capcity];
}

void myarr::push_back(int val)
{
    this->_myarraderss[this->_size]=val;
    this->_size++;
}
int myarr::getdata(int index)
{
    return this->_myarraderss[index];
}
void  myarr::setdata(int index,int val)
{
    this->_myarraderss[index]=val;
}

myarr::~myarr()
{
    if(this->_myarraderss!=nullptr)
    {
        cout<<"xigou调用"<<endl;
        delete[] this->_myarraderss;
        this->_myarraderss=nullptr;
    }
}
