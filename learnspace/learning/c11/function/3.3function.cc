/* function 还可以函数入参*/
#include <iostream>
#include <functional>

void call_when_even(int x, const std::function<void(int)> &f)
{
    if (!(x & 1)) // x%2==0
    {
        f(x);
    }
}

void output(int x)
{
    std::cout << x << " ";
}

int main()
{
    for (int i = 0; i < 10; ++i)
    {
        call_when_even(i, output);
    }
    std::cout << std::endl;
    return 0;
}