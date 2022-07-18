#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

typedef struct task
{
    void(*function)(void*);
    void*arg;
}task;

typedef struct threadpool
{
    /*任务队列*/
    task* taskq;
    int queuecapcity;
    int queuesize;//当前任务个数
    int queuefront;//队头出数组
    int queuerear;//队尾 进数据

    pthread_t manger; //管理者id
    pthread_t * threadid;//工作者线程id
    int minnum;// 最小线程数
    int maxnum;//最大线程数
    int busynum;//工作者线程的个数
    int livenum;//存活线程个数
    int exitnum;//要销毁的线程数量
    pthread_mutex_t mutexpool;// 锁整个线程池
    pthread_mutex_t mutexbusynum;// 锁经常变化的busynum
    pthread_cond_t notempty;//是不是要销毁线程池，1为销毁，0不销毁
}threadpool;

//创建并初始化线程池

//销毁线程池

//给线程池添加任务

//获取线程池中工作的线程的个数

//获取线程池中活着的线程个数