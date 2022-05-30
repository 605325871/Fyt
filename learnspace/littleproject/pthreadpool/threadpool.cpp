#include "threadpool.h"
#include <iostream>
#include <string.h>
#include <string>
using namespace std;
threadpool::threadpool(int min, int max)
{
    do
    {
        Taskq = new TaskQueue;
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
            pthread_create(&this->threadid[i], NULL, worker, NULL);
        }
        return;
    } while (0);

    if (threadid)
        delete[] threadid;
    if (Taskq)
        delete Taskq;
}

void *threadpool::worker(void *arg)
{
}
void *threadpool:: manger(void *arg)
{
}