#pragma once
#include <queue>
#include <pthread.h>
using callback = void (*)(void *arg);
template<class T>
struct task
{
    task<T>()
    {
        function = nullptr;
        arg = nullptr;
    }
    task<T>(callback f, void *arg)
    {
        this->arg = (T*)arg;
        function = f;
    }
    callback function;
     T*arg;
};
template<class T>
class TaskQueue
{
private:
    std::queue<task<T>> m_taskq;
    pthread_mutex_t m_mutex;

public:
    TaskQueue();
    ~TaskQueue();
    //添加任务
    void addtask(task<T> tsk);
    void addtask(callback f, void *arg);
    //取出任务
    task<T> taketask();
    //获取当前任务的个数
    inline int tasknumber()
    {
        return m_taskq.size();
    }
};
