


@[TOC](文章目录


# 一、线程池是什么？
>线程池是一种对于，对于线程的复用的技术，当我们需要用到多线程来完成我们的工作时，对于线程的频繁==创建==与==销毁==会带来巨大的开销，为了减轻这种开销我们因此使用如此方式，

>我们要使得我们的线程保证在一个可以维持正常工作的数量，不会因为高峰任务的太多而崩溃，不会因为工作数量少而过多线程闲置占用资源


# 二、分析
1.对于一个线程池来说主体应大致由三部分组成 
* 用来存储所要处理任务的任务队列 
* 工作的线程（worker）用来进行对单个任务的处理，对于任务队列不停的检测，如果有任务就处理，没有就阻塞
* 管理者线程（manager）主要是用来检测整个线程的线程的数量与任务的对比，如果线程数量多于所要处理的任务，那么就对部分线程的数量进行减小以达到减小开销的目的，若多则反之

2.模块分析
1.main()
 >   创建线程池
    向线程池中添加任务，借助回调函数处理任务
    销毁线程池

2.pthreadpool_create()
>   创建线程池结构体指针
    初始化线程池结构体（n个变量）
    创建N个任务线程
    创建1个管理者线程
    失败时销毁所有创建空间

3.pthread_destory
    对于关闭属性参数置为1
    回收管理管理者线程
    唤醒阻塞的消费者线程，让其进行自杀
    释放堆内存,销毁锁变量
    释放线程池结构体指针
4.pthread_add 
   向任务队列中加入任务
5.threadeixt 
    让不符合条件的线程退出

剩下的部分可以需要再创建或者细分
# 三、模拟创建
* 对于任务队列的创建
```c
typedef struct Task
{
    void (*function)(void *arg); //一个回调函数
    void *arg;  //向上方的回调函数传参
} Task;
```
>任务队列基本由回调函数组成，这样子可以更加灵活的调用，不用因为参数的不同的而进行转换，第二个参数是对回调函数进行传参

* 线程池结构体
```c
struct ThreadPool
{
    // 对与任务队列的属性的定义
    Task *taskQ; //用数组的结构模拟队列
    int queueCapacity; // 容量
    int queueSize;     // 当前任务个数
    int queueFront;    // 队头 -> 取数据
    int queueRear;     // 队尾 -> 放数据

	//对于线程池参数的管理
    pthread_t managerID;       // 管理者线程ID
    pthread_t *threadIDs;      // 工作的线程ID 将工作中的线程id通过数组管理
    int minNum;                // 最小线程数量，在开始就创建好要工作线程数的最小数量
    int maxNum;                // 最大线程数量
    int busyNum;               // 工作的线程的个数
    int liveNum;               // 存活的线程的个数
    int exitNum;               // 要销毁的线程个数
    pthread_mutex_t mutexPool; // 锁整个的线程池
    pthread_mutex_t mutexBusy; // 锁对于临界区中经常需要读取改变的变量进行加锁
    pthread_cond_t notFull;    // 任务队列是不是满了
    pthread_cond_t notEmpty;   // 任务队列是不是空了

    int shutdown; // 是不是要销毁线程池, 销毁为1, 不销毁为0
};
typedef struct ThreadPool ThreadPool;
```
* **创始化**

```c
ThreadPool* threadPoolCreate(int min, int max, int queueSize)
{
    ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));
    do 
    {
        if (pool == NULL)
        {
            printf("malloc threadpool fail...\n");
            break;
        }
        //对于线程数组id进行初始化
        pool->threadIDs = (pthread_t*)malloc(sizeof(pthread_t) * max);
        if (pool->threadIDs == NULL)
        {
            printf("malloc threadIDs fail...\n");
            break;
        }
        memset(pool->threadIDs, 0, sizeof(pthread_t) * max);
        pool->minNum = min;
        pool->maxNum = max;
        pool->busyNum = 0;
        pool->liveNum = min;    // 和最小个数相等
        pool->exitNum = 0;

        if (pthread_mutex_init(&pool->mutexPool, NULL) != 0 ||
            pthread_mutex_init(&pool->mutexBusy, NULL) != 0 ||
            pthread_cond_init(&pool->notEmpty, NULL) != 0 ||
            pthread_cond_init(&pool->notFull, NULL) != 0)
        {
            printf("mutex or condition init fail...\n");
            break;
        }

        // 任务队列
        pool->taskQ = (Task*)malloc(sizeof(Task) * queueSize);
        pool->queueCapacity = queueSize;
        pool->queueSize = 0; //初始任务为0
        pool->queueFront = 0;//头尾指针都指向头部
        pool->queueRear = 0;

        pool->shutdown = 0;

        // 创建线程
        pthread_create(&pool->managerID, NULL, manager, pool);
        for (int i = 0; i < min; ++i)
        {
            pthread_create(&pool->threadIDs[i], NULL, worker, pool);
        }
        return pool;
    } while (0);

    // 释放资源
    //如果上面的任何一步内存的申请发生错误就要释放前面申请的资源
    if (pool && pool->threadIDs) free(pool->threadIDs);
    if (pool && pool->taskQ) free(pool->taskQ);
    if (pool) free(pool);

    return NULL;
}

```
>min 为所创建的最小线程数量  max 最大线程数量
queueusize为任务的数量

* **向线程池中加入任务**

```c
void threadPoolAdd(ThreadPool* pool, void(*func)(void*), void* arg)
{
    pthread_mutex_lock(&pool->mutexPool);
    while (pool->queueSize == pool->queueCapacity && !pool->shutdown)
    {
        // 阻塞生产者线程
        pthread_cond_wait(&pool->notFull, &pool->mutexPool);
    }
    if (pool->shutdown)
    {
        pthread_mutex_unlock(&pool->mutexPool);
        return;
    }
    // 添加任务
    pool->taskQ[pool->queueRear].function = func;
    pool->taskQ[pool->queueRear].arg = arg;
    pool->queueRear = (pool->queueRear + 1) % pool->queueCapacity;
    pool->queueSize++;

    pthread_cond_signal(&pool->notEmpty);
    pthread_mutex_unlock(&pool->mutexPool);
}
```
>第一个参数是线程池结构体指针，第二个参数是要向向线程池结构体中加入回调函数，第三个是向回调函数中传入参数

* **执行任务的函数**
  
void* worker(void* arg)
{
    ThreadPool* pool = (ThreadPool*)arg;

    while (1)
    {
        pthread_mutex_lock(&pool->mutexPool);
        // 当前任务队列是否为空
        while (pool->queueSize == 0 && !pool->shutdown)
        {
            // 阻塞工作线程
            pthread_cond_wait(&pool->notEmpty, &pool->mutexPool);

            // 判断是不是要销毁线程
            //当前如果符号减少的线程的条件
            //时，设置减少的线程量
            if (pool->exitNum > 0)
            {
                pool->exitNum--;
                if (pool->liveNum > pool->minNum)
                {
                    pool->liveNum--;
                    pthread_mutex_unlock(&pool->mutexPool);
                    threadExit(pool);
                }
            }
        }

        // 判断线程池是否被关闭了
        if (pool->shutdown)
        {
            pthread_mutex_unlock(&pool->mutexPool);
            threadExit(pool);
        }

        // 从任务队列中取出一个任务
        Task task;
        task.function = pool->taskQ[pool->queueFront].function;
        task.arg = pool->taskQ[pool->queueFront].arg;
        // 移动头结点
        pool->queueFront = (pool->queueFront + 1) % pool->queueCapacity;
        pool->queueSize--;
        // 解锁
        pthread_cond_signal(&pool->notFull);
        pthread_mutex_unlock(&pool->mutexPool);

        printf("thread %ld start working...\n", pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum++;
        pthread_mutex_unlock(&pool->mutexBusy);
        task.function(task.arg);
        free(task.arg);
        task.arg = NULL;

        printf("thread %ld end working...\n", pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexBusy);
    }
    return NULL;
}

* **管理者线程**
```c

void* manager(void* arg)
{
    ThreadPool* pool = (ThreadPool*)arg;
    while (!pool->shutdown)
    {
        // 每隔5s检测一次
        sleep(5);

        // 取出线程池中任务的数量和当前线程的数量
        pthread_mutex_lock(&pool->mutexPool);
        int queueSize = pool->queueSize;
        int liveNum = pool->liveNum;
        pthread_mutex_unlock(&pool->mutexPool);

        // 取出忙的线程的数量
        pthread_mutex_lock(&pool->mutexBusy);
        int busyNum = pool->busyNum;
        pthread_mutex_unlock(&pool->mutexBusy);

        // 添加线程
        // 任务的个数>存活的线程个数 && 存活的线程数<最大线程数
        if (queueSize > liveNum && liveNum < pool->maxNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            int counter = 0;
            for (int i = 0; i < pool->maxNum && counter < NUMBER
                && pool->liveNum < pool->maxNum; ++i)
            {
                if (pool->threadIDs[i] == 0)
                {
                    pthread_create(&pool->threadIDs[i], NULL, worker, pool);
                    counter++;
                    pool->liveNum++;
                }
            }
            pthread_mutex_unlock(&pool->mutexPool);
        }
        // 销毁线程
        // 忙的线程*2 < 存活的线程数 && 存活的线程>最小线程数
        if (busyNum * 2 < liveNum && liveNum > pool->minNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            pool->exitNum = NUMBER;
            pthread_mutex_unlock(&pool->mutexPool);
            // 让工作的线程自杀
            for (int i = 0; i < NUMBER; ++i)
            {
                pthread_cond_signal(&pool->notEmpty);
            }
        }
    }
    return NULL;
}
```
>管理者线程的任务主要是对任务数量与对线程数量的检查，判断增加或减少线程，更具体的减少与增加的判断的条件可以根据条件设置


* **设置让线程减少的函数**
```c
void threadExit(ThreadPool* pool)
{
    pthread_t tid = pthread_self();
    for (int i = 0; i < pool->maxNum; ++i)
    {
        if (pool->threadIDs[i] == tid)
        {
            pool->threadIDs[i] = 0;
            printf("threadExit() called, %ld exiting...\n", tid);
            break;
        }
    }
    pthread_exit(NULL);
}
  ```
  >主要功能是依据前面的所设置线程数组id进行线程的推出


  完整代码
  ```c
  #include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
const int NUMBER = 0;
void *manager(void *arg);
void *worker(void *arg);




typedef struct ThreadPool ThreadPool;
// 创建线程池并初始化
ThreadPool *threadPoolCreate(int min, int max, int queueSize);

// 销毁线程池
int threadPoolDestroy(ThreadPool* pool);

// 给线程池添加任务
void threadPoolAdd(ThreadPool* pool, void(*func)(void*), void* arg);

// 获取线程池中工作的线程的个数
int threadPoolBusyNum(ThreadPool* pool);

// 获取线程池中活着的线程的个数
int threadPoolAliveNum(ThreadPool* pool);

//////////////////////
// 工作的线程(消费者线程)任务函数
void* worker(void* arg);
// 管理者线程任务函数
void* manager(void* arg);
// 单个线程退出
void threadExit(ThreadPool* pool);

// 任务结构体
typedef struct Task
{
    void (*function)(void *arg);
    void *arg;
} Task;
// 线程池结构体
struct ThreadPool
{
    // 任务队列
    Task *taskQ;
    int queueCapacity; // 容量
    int queueSize;     // 当前任务个数
    int queueFront;    // 队头 -> 取数据
    int queueRear;     // 队尾 -> 放数据

    pthread_t managerID;       // 管理者线程ID
    pthread_t *threadIDs;      // 工作的线程ID
    int minNum;                // 最小线程数量
    int maxNum;                // 最大线程数量
    int busyNum;               // 忙的线程的个数
    int liveNum;               // 存活的线程的个数
    int exitNum;               // 要销毁的线程个数
    pthread_mutex_t mutexPool; // 锁整个的线程池
    pthread_mutex_t mutexBusy; // 锁busyNum变量
    pthread_cond_t notFull;    // 任务队列是不是满了
    pthread_cond_t notEmpty;   // 任务队列是不是空了

    int shutdown; // 是不是要销毁线程池, 销毁为1, 不销毁为0
};

typedef struct ThreadPool ThreadPool;
// 创建线程池并初始化
void perr_exit(const char *S)
{
    perror(S);
    exit(1);
}
ThreadPool* threadPoolCreate(int min, int max, int queueSize)
{
    ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));
    do 
    {
        if (pool == NULL)
        {
            printf("malloc threadpool fail...\n");
            break;
        }

        pool->threadIDs = (pthread_t*)malloc(sizeof(pthread_t) * max);
        if (pool->threadIDs == NULL)
        {
            printf("malloc threadIDs fail...\n");
            break;
        }
        memset(pool->threadIDs, 0, sizeof(pthread_t) * max);
        pool->minNum = min;
        pool->maxNum = max;
        pool->busyNum = 0;
        pool->liveNum = min;    // 和最小个数相等
        pool->exitNum = 0;

        if (pthread_mutex_init(&pool->mutexPool, NULL) != 0 ||
            pthread_mutex_init(&pool->mutexBusy, NULL) != 0 ||
            pthread_cond_init(&pool->notEmpty, NULL) != 0 ||
            pthread_cond_init(&pool->notFull, NULL) != 0)
        {
            printf("mutex or condition init fail...\n");
            break;
        }

        // 任务队列
        pool->taskQ = (Task*)malloc(sizeof(Task) * queueSize);
        pool->queueCapacity = queueSize;
        pool->queueSize = 0;
        pool->queueFront = 0;
        pool->queueRear = 0;

        pool->shutdown = 0;

        // 创建线程
        pthread_create(&pool->managerID, NULL, manager, pool);
        for (int i = 0; i < min; ++i)
        {
            pthread_create(&pool->threadIDs[i], NULL, worker, pool);
        }
        return pool;
    } while (0);

    // 释放资源
    if (pool && pool->threadIDs) free(pool->threadIDs);
    if (pool && pool->taskQ) free(pool->taskQ);
    if (pool) free(pool);

    return NULL;
}

int threadPoolDestroy(ThreadPool* pool)
{
    if (pool == NULL)
    {
        return -1;
    }

    // 关闭线程池
    pool->shutdown = 1;
    // 阻塞回收管理者线程
    pthread_join(pool->managerID, NULL);
    // 唤醒阻塞的消费者线程
    for (int i = 0; i < pool->liveNum; ++i)
    {
        pthread_cond_signal(&pool->notEmpty);
    }
    // 释放堆内存
    if (pool->taskQ)
    {
        free(pool->taskQ);
    }
    if (pool->threadIDs)
    {
        free(pool->threadIDs);
    }

    pthread_mutex_destroy(&pool->mutexPool);
    pthread_mutex_destroy(&pool->mutexBusy);
    pthread_cond_destroy(&pool->notEmpty);
    pthread_cond_destroy(&pool->notFull);

    free(pool);
    pool = NULL;

    return 0;
}


void threadPoolAdd(ThreadPool* pool, void(*func)(void*), void* arg)
{
    pthread_mutex_lock(&pool->mutexPool);
    while (pool->queueSize == pool->queueCapacity && !pool->shutdown)
    {
        // 阻塞生产者线程
        pthread_cond_wait(&pool->notFull, &pool->mutexPool);
    }
    if (pool->shutdown)
    {
        pthread_mutex_unlock(&pool->mutexPool);
        return;
    }
    // 添加任务
    pool->taskQ[pool->queueRear].function = func;
    pool->taskQ[pool->queueRear].arg = arg;
    pool->queueRear = (pool->queueRear + 1) % pool->queueCapacity;
    pool->queueSize++;

    pthread_cond_signal(&pool->notEmpty);
    pthread_mutex_unlock(&pool->mutexPool);
}

int threadPoolBusyNum(ThreadPool* pool)
{
    pthread_mutex_lock(&pool->mutexBusy);
    int busyNum = pool->busyNum;
    pthread_mutex_unlock(&pool->mutexBusy);
    return busyNum;
}

int threadPoolAliveNum(ThreadPool* pool)
{
    pthread_mutex_lock(&pool->mutexPool);
    int aliveNum = pool->liveNum;
    pthread_mutex_unlock(&pool->mutexPool);
    return aliveNum;
}

void* worker(void* arg)
{
    ThreadPool* pool = (ThreadPool*)arg;

    while (1)
    {
        pthread_mutex_lock(&pool->mutexPool);
        // 当前任务队列是否为空
        while (pool->queueSize == 0 && !pool->shutdown)
        {
            // 阻塞工作线程
            pthread_cond_wait(&pool->notEmpty, &pool->mutexPool);

            // 判断是不是要销毁线程
            if (pool->exitNum > 0)
            {
                pool->exitNum--;
                if (pool->liveNum > pool->minNum)
                {
                    pool->liveNum--;
                    pthread_mutex_unlock(&pool->mutexPool);
                    threadExit(pool);
                }
            }
        }

        // 判断线程池是否被关闭了
        if (pool->shutdown)
        {
            pthread_mutex_unlock(&pool->mutexPool);
            threadExit(pool);
        }

        // 从任务队列中取出一个任务
        Task task;
        task.function = pool->taskQ[pool->queueFront].function;
        task.arg = pool->taskQ[pool->queueFront].arg;
        // 移动头结点
        pool->queueFront = (pool->queueFront + 1) % pool->queueCapacity;
        pool->queueSize--;
        // 解锁
        pthread_cond_signal(&pool->notFull);
        pthread_mutex_unlock(&pool->mutexPool);

        printf("thread %ld start working...\n", pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum++;
        pthread_mutex_unlock(&pool->mutexBusy);
        task.function(task.arg);
        free(task.arg);
        task.arg = NULL;

        printf("thread %ld end working...\n", pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexBusy);
    }
    return NULL;
}

void* manager(void* arg)
{
    ThreadPool* pool = (ThreadPool*)arg;
    while (!pool->shutdown)
    {
        // 每隔3s检测一次
        sleep(3);

        // 取出线程池中任务的数量和当前线程的数量
        pthread_mutex_lock(&pool->mutexPool);
        int queueSize = pool->queueSize;
        int liveNum = pool->liveNum;
        pthread_mutex_unlock(&pool->mutexPool);

        // 取出忙的线程的数量
        pthread_mutex_lock(&pool->mutexBusy);
        int busyNum = pool->busyNum;
        pthread_mutex_unlock(&pool->mutexBusy);

        // 添加线程
        // 任务的个数>存活的线程个数 && 存活的线程数<最大线程数
        if (queueSize > liveNum && liveNum < pool->maxNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            int counter = 0;
            for (int i = 0; i < pool->maxNum && counter < NUMBER
                && pool->liveNum < pool->maxNum; ++i)
            {
                if (pool->threadIDs[i] == 0)
                {
                    pthread_create(&pool->threadIDs[i], NULL, worker, pool);
                    counter++;
                    pool->liveNum++;
                }
            }
            pthread_mutex_unlock(&pool->mutexPool);
        }
        // 销毁线程
        // 忙的线程*2 < 存活的线程数 && 存活的线程>最小线程数
        if (busyNum * 2 < liveNum && liveNum > pool->minNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            pool->exitNum = NUMBER;
            pthread_mutex_unlock(&pool->mutexPool);
            // 让工作的线程自杀
            for (int i = 0; i < NUMBER; ++i)
            {
                pthread_cond_signal(&pool->notEmpty);
            }
        }
    }
    return NULL;
}

void threadExit(ThreadPool* pool)
{
    pthread_t tid = pthread_self();
    for (int i = 0; i < pool->maxNum; ++i)
    {
        if (pool->threadIDs[i] == tid)
        {
            pool->threadIDs[i] = 0;
            printf("threadExit() called, %ld exiting...\n", tid);
            break;
        }
    }
    pthread_exit(NULL);
}

void taskFunc(void *arg)
{
    int num = *(int *)arg;
    printf("thread %ld is working, number = %d\n",
           pthread_self(), num);
    sleep(1);
}

int main()
{
    // 创建线程池
    ThreadPool *pool = threadPoolCreate(3, 10, 100);
    for (int i = 0; i < 100; ++i)
    {
        int *num = (int *)malloc(sizeof(int));
        *num = i + 100;
        threadPoolAdd(pool, taskFunc, num);
    }

    sleep(30);

    threadPoolDestroy(pool);
    return 0;
}

  ```


