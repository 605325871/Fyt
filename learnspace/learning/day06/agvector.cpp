#include<iostream>
#include<string>
using namespace std;
#include<vector>
# define max 10
void printvector(vector<int> &v1)
{
    for(vector<int>::iterator it=v1.begin();it!=v1.end();it++)
    {
        cout<<*it;
    }
    cout<<endl;
}

// 
void test01()
{   
    //默认构造
    vector<int> v;
    int i;
    for(i=0; i<max;i++)
    {
        v.push_back(i);
    }
    printvector(v);
    //通过区间的方式去构造
    vector<int> v2(v.begin(),v.end());
    printvector(v2);
    //n个elem的方式去构造
    vector<int> v3(10,100);
    printvector(v3);
    cout<<"v2 的size = "<<v2.size()<<" v2 的capcity = "<<v2.capacity()<<endl;
    if(!v2.empty())
    {
        cout<<"不是空的"<<endl;
    }
}
int main()
{
    test01();
}