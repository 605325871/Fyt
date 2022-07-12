#include <thread>
#include <iostream>

using namespace std;

int main()
{
    std::thread t1; //空线程
    cout << t1.get_id() << endl;
    return 0;
}