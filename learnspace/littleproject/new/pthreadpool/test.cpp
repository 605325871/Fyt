
#include "threadpool.h"
#include<unistd.h>
#include<stdio.h>
#include"threadpool.cpp" //模板分文件编写
using namespace std;
void taskfunc(void *arg)
{
    int num = *(int *)arg;
    printf("thread %ld is working, number = %d\n",
        pthread_self(), num);
        sleep(1);
}

int main()
{
    // 创建线程池
    threadpool<int> pool(3, 10);
    for (int i = 0; i < 100; ++i)
    {
        int *num =  new int(i+100);
       pool.addTask(task<int>(taskfunc, num));
    }

    sleep(30);

    return 0;
}
