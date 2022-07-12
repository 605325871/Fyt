#include <iostream>
#include <thread>
using namespace std;

class A
{
public:
    void func1(int x)
    {
        cout << x << endl;
    }
    static void func2(int x)
    {
        cout << x << endl;
    }
};

int main()
{
    A a;
    thread t1(&A::func1, a, 10); // 普通成员函数需要传入类的实例或者指针
    t1.join();

    thread t2(&A::func2,10); //静态成员函数则不需要
    t2.join();
    return 0;
}