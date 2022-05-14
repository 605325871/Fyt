#include<iostream>
using namespace std;

class person
{
private:
    /* data */
public:
    person(/* args */){};
    ~person(){};
    //一般不会用成员函数重载 《《 运算符 因为这样的调用就会 p《《cout 与使用顺序不符
    int _a;
    int _b;
};
ostream & operator<<(ostream &out ,person& p)
{
    cout<<"m_A = "<<p._a<<" m_ b = "<<p._b;
    return out;
}
void test01()
{
    person p;
    p._a=10;
    p._b=10;
    cout<<p<<endl;
}


int main()
{
    test01();
}