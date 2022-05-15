#include<iostream>
#include<vector>
#include<string>
using namespace std;

class person
{
public:
    person(/* args */) {}
    person(string name ,int age) {
        this->_name=name;
        this->_age=age;
    }
    ~person() {}
    int _age;
    string _name;
};

void test01()
{
    person  p1("zhangsan",18);
    person  p2("lisi",19);
    person  p3("wangwu",22);
    vector<person> v1;
    v1.push_back(p1);
    v1.push_back(p2);
    v1.push_back(p3);
    for(vector<person>::iterator it=v1.begin();it<v1.end();it++)
    {
        cout<<"姓名"<<(*it)._name<<" 年龄"<<(*it)._name<<endl;
    }

}
int main()
{
    test01();
}
