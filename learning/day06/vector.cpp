#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

void test01()
{
   int i;
   vector<int> v;
   for(i=0;i<10;i++)
   {
       v.push_back(i);
   }

//    for(vector<int>::iterator it =v.begin(); it!=v.end();it++)
//    {
//        cout<<*it <<endl;
//    }
   // for_each(v.begin(),v.end())
   
}
int main()
{
    test01();
}