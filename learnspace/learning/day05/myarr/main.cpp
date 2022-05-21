#include"myarr.h"
// #include<iostream>
// using namespace std;

void test01()
{
    myarr* myar =new myarr(30);

    myarr* arry2(myar);
 
    for(int i=0 ; i<10; i++)
    {
        arry2->push_back(i);
    }
     for(int i=0 ; i<10; i++)
    {
       cout<<arry2->getdata(i)<<endl;
    }
 delete(myar);
}
int main()
{
    test01();
}