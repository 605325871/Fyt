#include "threadpool.h"
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>
//模板分文件编写
using namespace std;
template<class T>
threadpool<T>::threadpool(int min, int max)
{
    do
    {
        Taskq = new TaskQueue<T>;
        if (Taskq == nullptr)
        {
            cout << "Taskq new error" << endl;
            break;
        }
        this->threadid = new pthread_t[max];
        if (threadid == nullptr)
        {
            cout << "new error" << endl;
            break;
        }
        memset(threadid, 0, sizeof(pthread_t) * max);
        this->busyNum = 0;
        this->exitNum = 0;
        this->minNum = min;
        this->maxNum = max;

        if (pthread_mutex_init(&this->mutepool, NULL) != 0 ||
            pthread_cond_init(&this->notempty, NULL) != 0)
        {
            cout << "mutex or condition error" << endl;
            break;
        }

        this->shutdown = false;

        pthread_create(&managerid, NULL, manger, this);

        for (int i = 0; i < min; i++)
        {
            pthread_create(&this->threadid[i], NULL, worker, this);
        }
        return;
    } while (0);

    if (threadid)
        delete[] threadid;
    if (Taskq)
        delete Taskq;
}
template<class T>
threadpool<T>::~threadpool()
{
    this->shutdown = true;

    pthread_join(this->managerid, NULL);

    for (int i = 0; i < this->liveNum; i++)
    {
        pthread_cond_signal(&notempty);
    }
    if (Taskq)
    {
        delete Taskq;
    }
    if (threadid)
    {
        delete[] threadid;
    }
    pthread_mutex_destroy(&mutepool);
    pthread_cond_destroy(&notempty);
}
template<class T>
void threadpool<T>::threadexit()
{
    pthread_t tid = pthread_self();

    for (int i = 0; i < maxNum; i++)
    {
        if (threadid[i] == tid)
        {
            threadid[i] = 0;
            cout << " thread " << to_string(tid) << " exiting----" << endl;
            break;
        }
    }
    pthread_exit(NULL);
}
template<class T>
void *threadpool<T>::worker(void *arg)
{
    //非静态函数是无法正常访问成员变量
    threadpool *pool = static_cast<threadpool *>(arg);
    while (true)
    {
        //访问任务队列加锁
        pthread_mutex_lock(&pool->mutepool);
        //判断任务队列是否为空，如果为空工作线程阻塞
        while (pool->Taskq->tasknumber() == 0 && !pool->shutdown)
        {
            cout << " thread " << to_string(pthread_self()) << "waiting ---" << endl;

            pthread_cond_wait(&pool->notempty, &pool->mutepool);

            if (pool->exitNum > 0)
            {
                pool->exitNum--;
                if (pool->liveNum > pool->minNum)
                {
                    pool->liveNum--;
                    pthread_mutex_unlock(&pool->mutepool);
                    pool->threadexit();
                }
            }

            if (pool->shutdown)
            {
                pthread_mutex_unlock(&pool->mutepool);
                pool->threadexit();
            }
            pool->busyNum++;
            task<T> task = pool->Taskq->taketask();
            pthread_mutex_unlock(&pool->mutepool);

            cout << "thread " << to_string(pthread_self()) << "strat working " << endl;
            task.function(task.arg);
            delete task.arg;
            task.arg = nullptr;
            cout << "thread " << to_string(pthread_self()) << "end working " << endl;

            pthread_mutex_lock(&pool->mutepool);
            pool->busyNum--;
            pthread_mutex_unlock(&pool->mutepool);
        }
    }
}
template<class T>
void *threadpool<T>::manger(void *arg)
{
    threadpool *pool = static_cast<threadpool *>(arg);

    while (!pool->shutdown)
    {
        sleep(3);

        pthread_mutex_lock(&pool->mutepool);
        int queuesize = pool->Taskq->tasknumber();
        int livenum = pool->liveNum;
        int busynum = pool->busyNum;
        pthread_mutex_unlock(&pool->mutepool);
        if (queuesize > livenum && livenum < pool->maxNum)
        {
            pthread_mutex_lock(&pool->mutepool);
            int counter = 0;

            for (int i = 0; i < pool->maxNum && counter < NUMBRE && pool->liveNum < pool->maxNum; ++i)
            {
                if (pool->threadid[i] == 0)
                {
                    pthread_create(&pool->threadid[i], NULL, worker, pool);
                    counter++;
                    pool->liveNum++;
                }
            }
            pthread_mutex_unlock(&pool->mutepool);
        }

        if (busynum * 2 < livenum && livenum > pool->minNum)
        {
            pthread_mutex_lock(&pool->mutepool);
            pool->exitNum = NUMBRE;
            pthread_mutex_unlock(&pool->mutepool);

            for (int i = 0; i < NUMBRE; ++i)
            {
                pthread_cond_signal(&pool->notempty);
            }
        }
    }
    return NULL;
}
template<class T>
void threadpool<T>::addTask(task<T> task)
{
    if (shutdown)
    {
        return;
    }
    Taskq->addtask(task);
    pthread_cond_signal(&notempty);
}
template<class T>
int threadpool<T>::getBusyNum()
{
    pthread_mutex_lock(&mutepool);
    int busynum = this->busyNum;
    pthread_mutex_lock(&mutepool);
    return busynum;
}
template<class T>
int threadpool<T>::getliveNum()
{
    pthread_mutex_lock(&mutepool);
    int livenum = this->liveNum;
    pthread_mutex_lock(&mutepool);
    return livenum;
}