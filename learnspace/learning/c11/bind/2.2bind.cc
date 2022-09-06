#include <iostream>
#include <functional>

class A
{
private:
    /* data */
public:
    int _i = 0;
    void output(int x, int y)
    {
        std::cout << x << " " << y << " " << std::endl;
    }
};

int main()
{
    A a;
    std::function<void(int, int)> fr = std::bind(&A::output, &a, std::placeholders::_1, std::placeholders::_2);
    fr(1, 2);

    std::function<int &(void)> fr_i = std::bind(&A::_i, &a);
    fr_i() = 123;

    std::cout << a._i << std::endl;
    return 0;
}