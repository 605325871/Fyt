#include <iostream>
#include <thread>
using namespace std;
#include <atomic>

atomic_long sum{ 0 };
void fun(size_t num)
{
	for (size_t i = 0; i < num; ++i)
		sum++; // 原子操作
}
int main()
{
	cout << "Before joining, sum = " << sum << std::endl;
	thread t1(fun, 1000000);
	thread t2(fun, 1000000);
	t1.join();
	t2.join();
	cout << "After joining, sum = " << sum << std::endl;
	return 0;
}