#include<iostream>
#include<string>
#include<set>
using namespace std;

class person
{
private:
    /* data */
public:
    person(/* args */) {}
    person(string name,int age)
    :_name(name),_age(age)
    {}
    ~person() {}
    string _name;
    int   _age;
};

class comperperson
{
public:
    bool operator()(const person&p1,const person&p2)
    {
        return p1._age<p2._age;
    }
};

int main()
{
    set<person,comperperson> s;
    person p1("zhangsan",30);
    person p2("lisi",40);
    person p3("wangwu",50);

    s.insert(p1);
    s.insert(p2);
    s.insert(p3);

    for(auto item : s)
    cout<<" 姓名： "<<item._name<<" 年龄 : "<<item._age<<endl;
    return 0;
    
}