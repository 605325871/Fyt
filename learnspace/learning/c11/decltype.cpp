#include<iostream>
#include<initializer_list>
#include<vector>
#include<typeinfo>
using namespace std;

void* getmemory(size_t size)
{
    return malloc(size);
}

int main()
{
    char h='d';
    //如果没有带参数则推导的时函数的类型
    cout<<typeid(decltype(getmemory)).name()<<endl;
    
    
    cout<<typeid(decltype(getmemory(0))).name()<<endl;

     vector<int> v;
     for(int i=0;i<10;i++)
     {
         v.push_back(i+1);
     }
     for(auto item:v)
     {
      cout<<item<<endl;   
     }
    /*
    for (declaration : expression)
{
    // 循环体
}

    */
   

    
}