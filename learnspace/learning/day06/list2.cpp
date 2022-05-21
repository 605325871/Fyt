#include<iostream>
#include<list>
using namespace std;
void printlist(list<int>& l)
{
    for(list<int>::const_iterator it =l.begin();it!=l.end();it++)
        cout<<*it<<" ";
    cout<<endl;
}
void test01()
{
}
int main()
{
    
}