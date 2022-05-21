#include<iostream>
#include<string>
#include<vector>
using namespace std;
#define size 10
void test01()
{
    vector<int> v;
    int i;
    for(i=0;i<size;i++)
    {
        v.push_back(rand()%100);
    }
    for(i=0;i<size ;i++)
    {
        cout<<v[i]<<" "<<v.at(i);
    }
    cout<<endl;

    //互换容器
}
void test02()
{
    vector<int> v;
    int i;
    int *p=NULL;
    int cnt=0;
    v.reserve(100000);
    for(i=0; i<100000;i++)
    {
        v.push_back(i);
        if(p!=&v[0])
        {
            p=&v[0];
            cnt++;
        }
    }

    cout<<cnt<<endl;
    
}
int main()
{
    test02();
}