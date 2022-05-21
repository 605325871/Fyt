#include<iostream>
using namespace std;
//运算符重载
class person
{

public:
    person(){};
    person(int a,int b);
    ~person();
    person operator+(person &p)
    {
    person temp;
    temp._a=this->_a+p._a;
    temp._b=this->_b+p._b;
    return temp;

    }
    int _a;
    int _b;
};

person::person(int a,int b)
:_a(a)
,_b(b)
{}

person::~person()
{
}
void test01()
{
    person p1(10,10);
    person p2(10,10);
    person p3 = p1+p2;
    cout<<" a= "<<p3._a<<"  b = "<<p3._b<<endl;
}

int main()
{
    test01();
    return 0;
}