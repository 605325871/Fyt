#include<iostream>
#include<string>
using namespace std;
template<class nametype,class datatype>
class person
{
public:
    person(nametype name,datatype age) {
        this->_age=age;
        this->_name=name;
    }
    ~person() {}
    void showperson()
    {
        cout<<"name :  "<<this->_name<<"  age: "<<this->_age<<endl;
    }
    nametype _name;
    datatype _age;
};
void test01()
{
    person<string,int> p("zhangsan",18);
    p.showperson();
}
int main()
{
    test01();
}