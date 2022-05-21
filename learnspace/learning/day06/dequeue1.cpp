#include<iostream>
#include<stack>
using namespace std;
#include<queue>
void test01()
{
    stack<int> stk;
    stk.push(10);
    stk.push(20);
    stk.push(20);
    while(!stk.empty())
    {
        cout<<"  a = "<<stk.top()<<endl;
        stk.pop();
    }
    
}
class person
{
public:
    person(string name,int age)
    :_name(name),_age(age)
    {}
    int _age;
    string _name;
};
void test02()
{
    queue<person> q;
    person p1("zhangsan",30);
    person p2("lisi",40);
    person p3("wangwu",50);
    q.push(p1);
    q.push(p2);
    q.push(p3);
    while(!q.empty())
    {
        cout<<" head name : "<<q.front()._name<<"head age : "<<q.front()._age<<endl;
        cout<<"back name : "<<q.back()._name<<" back age : "<<q.back()._age<<endl;
        q.pop();
    }
    cout<<q.size()<<endl;
}
int main()
{
    test02();
}
