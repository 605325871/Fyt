#include <thread>
#include <iostream>
using namespace std;

void func1(int &x)
{
    x += 10;
    return;
}
void func2(int *x)
{
    *x += 10;
    return;
}

int main()
{
    int a = 10;
    thread t1(func1, ref(a));
    t1.join();
    cout << a << endl;
    thread t2(func1, &a);
    t2.join();
    cout << a << endl;
}