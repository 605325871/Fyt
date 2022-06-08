#include <iostream>
using namespace std;

/*
    constexper函数的限制：
     *函数中 只能有一个return语句（可以使用typedef与using语句与静态断言
     *函数必须返回值（不能是void函数）
     *在使用前必须已有定义（函数主体写在要使用的函数前面）
     *return返回语句表达式中不能使用非常量表达式的函数，全局数据，且必须是一个常量表达式
*/

constexpr int GetNum()
{
    return 3;
}

enum class B : unsigned int
{
    okm,
    error
};

class A
{
private:
    int _a;

public:
    A()=default;
    A(int a)
        : _a(a) {}
    ~A() {}
};
int main()
{
    //强枚举类型，enum后面加上class或者struct修饰，解决了重定义的变量
    // enum 可以指定内置数据类型
    // enum {e1=GetNum(),e2};
    enum class status
    {
        okm = GetNum(),
        error
    };
    enum class status2
    {
        okm,
        error
    };

    A a;
}