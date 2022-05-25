#pragma once
#include <queue>
#include <pthread.h>
using callback = void (*)(void *arg);
struct task
{
    task()
    {
        function = nullptr;
        arg = nullptr;
    }
    task(callback f, void *arg)
    {
        this->arg = arg;
        function = f;
    }
    callback function;
    void *arg;
};

class TaskQueue
{
private:
    std::queue<task> m_taskq;
    pthread_mutex_t m_mutex;

public:
    TaskQueue();
    ~TaskQueue();
    //添加任务
    void addtask(task tsk);
    void addtask(callback f, void *arg);
    //取出任务
    task taketask(task tsk);
    //获取当前任务的个数
    inline int tasknumber()
    {
        return m_taskq.size();
    }
};
