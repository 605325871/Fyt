#include "TaskQueue.h"
template<class T>
TaskQueue<T>::TaskQueue(/* args */)
{
    pthread_mutex_init(&m_mutex, NULL);
}
template<class T>
TaskQueue<T>::~TaskQueue()
{
    pthread_mutex_destroy(&m_mutex);
}
//添加任务
template<class T>
void TaskQueue<T>::addtask(task<T> tsk)
{
    pthread_mutex_lock(&m_mutex);
    m_taskq.push(tsk);
    pthread_mutex_unlock(&m_mutex);
}
template<class T>
void TaskQueue<T>::addtask(callback f, void *arg)
{
    pthread_mutex_lock(&m_mutex);
    m_taskq.push(task<T>(f,arg));
    pthread_mutex_unlock(&m_mutex);
}

//取出任务
template<class T>
task<T> TaskQueue<T>::taketask()
{
    task<T> t;
    pthread_mutex_lock(&m_mutex);
    if (!m_taskq.empty())
    {
        t = m_taskq.front();
        m_taskq.pop();
    }
    pthread_mutex_unlock(&m_mutex);
    return t;
}