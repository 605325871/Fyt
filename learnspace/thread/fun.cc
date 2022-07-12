#include <thread>
#include <iostream>
using namespace std;

void ThreadFunc(int a)
{
    cout << "thread 1" << endl;
}

class TF
{
public:
    void operator()()
    {
        cout << "thread3" << endl;
    }
};

int main()
{
    thread t1(ThreadFunc, 10); ///线程函数为函数指针
    thread t2([]()
              { cout << "thread 2" << endl; }); //线程函数为lambda表达式
    //线程函数为函数对象
    TF tf;
    thread t3(tf);
    t1.join();
    t2.join();
    t3.join();
    cout << "main thread :" << endl;
    return 0;
}