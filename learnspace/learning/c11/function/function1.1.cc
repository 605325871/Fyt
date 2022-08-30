#include <iostream>
#include <functional>

void func(void)
{
    std::cout << __FUNCTION__ << std::endl;
}

class foo
{
private:
    /* data */
public:
    static int foo_func(int a)
    {
        std::cout << __FUNCTION__ << " ( " << a << std::endl;
        return a;
    }
};

class bar
{
private:
    /* data */
public:
    int operator()(int a)
    {
        std::cout << __FUNCTION__ << " ( " << a << std::endl;
        return a;
    }
};

int main()
{
    std::function<void(void)> fr1 = func; //绑定一个普通函数

    fr1();

    std::function<int(int)> fr2 = foo::foo_func; //绑定一个类的静态成员函数
    std::cout << fr2(123) << std::endl;

    bar bar;
    fr2 = bar;
    std::cout << fr2(123) << std::endl;

    return 0;
}