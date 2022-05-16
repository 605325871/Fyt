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
    list<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    printlist(l1);
}
int main()
{
    test01();
}