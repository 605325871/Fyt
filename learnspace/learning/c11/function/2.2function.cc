#include <iostream>
#include <functional>

class A
{
    std::function<void()> callback_;

public:
    A(const std::function<void()> &f) : callback_(f)
    {
    }

    void notify(void)
    {
        callback_(); //回调到上层
    }
};

class foo
{
private:
    /* data */
public:
    void operator()(void)
    {
        std::cout << __FUNCTION__ << std::endl;
    }
};

int main(void)
{
    foo foo;
    A aa(foo);
    aa.notify();

    return 0;
}