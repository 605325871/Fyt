#include <iostream>
using namespace std;

class A
{
public:
    A(int a, int b)
        : _a(a), _b(b)
    {}
    ~A() {}

protected:
    int _a;
    int _b;
};
//允许派生类继承默认的构造函数，但是不能为私有，且不能为虚继承
//一旦使用继承构造，编译器不会再为派生类产生默认构造函数
class B :public A
{
public:
#if 0
    B(int x,int y)
    :A(x,y) //通过默认参数列表给基类传参
    {}
#endif
    using A::A;//使用A的构造函数进行传参
    void display()
    {
        cout<<" _a = "<<_a<<"b = "<<_b<<endl;
    }
};

int main()
{
    B obj(10,20);

}