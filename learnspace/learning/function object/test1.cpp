#include <iostream>
#include <string>
using namespace std;

/**
 * 函数对象（仿函数）
 * 函数对象在使用时可以向普通函数那样调用，可以有参数，可以有返回值
 * 函数对象可以
 */
class myadd
{
public:
    int operator()(int v1, int v2) //对函数调用符重载
    {
        return v1 + v2;
    }
};

class myprint
{
public:
    myprint()
    {
        this->count = 0;
    }
    void operator()(string test)
    {
        cout << test << endl;
        this->count++;
    }
    int count = 0;
};
void doprint(myprint &mp, string test)
{
    mp(test);
}
void test01()
{
    myprint myp;
    doprint(myp, "aaaaaa");
}
int main()
{
    test01();
}