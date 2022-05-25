#include "TaskQueue.h"

TaskQueue::TaskQueue(/* args */)
{
    pthread_mutex_init(&m_mutex, NULL);
}
TaskQueue::~TaskQueue()
{
    pthread_mutex_destroy(&m_mutex);
}
//添加任务
void TaskQueue::addtask(task tsk)
{
    pthread_mutex_lock(&m_mutex);
    m_taskq.push(tsk);
    pthread_mutex_unlock(&m_mutex);
}
void TaskQueue::addtask(callback f, void *arg)
{
    pthread_mutex_lock(&m_mutex);
    m_taskq.push(task(f,arg));
    pthread_mutex_unlock(&m_mutex);
}
//取出任务
task TaskQueue::taketask(task tsk)
{
    task t;
    pthread_mutex_lock(&m_mutex);
    if (!m_taskq.empty())
    {
        t = m_taskq.front();
        m_taskq.pop();
    }
    pthread_mutex_unlock(&m_mutex);
    return t;
}