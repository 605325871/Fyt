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
    vector<int> v2;
    v2=v;
    printvector(v);

    //reszie重新制定大小
    v2.resize(15,100);//默认用0 填充，也可以指定其他字符
    //如果短了就删除掉多的部分
    
}
int main()
{
    test01();
}