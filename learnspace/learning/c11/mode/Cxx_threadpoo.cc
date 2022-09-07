#include <list>
#include <thread>
#include <memory>
#include <functional>
#include <atomic>
#include "obMode.hpp"

const int MaxTaskCount = 100;

class Cxx_threadpoo
{
public:
    using task = std::function<void()>;
    Cxx_threadpoo(int numthreads = std::thread::hardware_concurrency()) : m_queue(MaxTaskCount)
    {
        Start(numthreads);
    }
    ~Cxx_threadpoo()
    {
        Stop();
    }
    void Stop()
    {
        //保证多线程情况下只调用一次stopflag
        std::call_once(m_flag, [this]
                       { return StopThreadGroup(); });
    }
    void AddTask(task &&taskt)
    {
        m_queue.Put(std::forward<task>(taskt));
    }
    void AddTask(const task &task1)
    {
        m_queue.Put(task1);
    }

private:
    void Start(int numthreads)
    {
        m_running = true;
        for (int i = 0; i < numthreads; ++i)
        {
            m_threadGoup.push_back(std::make_shared<std::thread>(&Cxx_threadpoo::runInThread, this));
        }
    }
    void runInThread()
    {
        while (m_running)
        {
            std::list<task> list;
            m_queue.Take(list);
            for (auto &task : list)
            {
                if (!m_running)
                {
                    return;
                }

                task();
            }
        }
    }
    void StopThreadGroup()
    {
        m_queue.Stop();
        m_running = false;

        for (auto thread : m_threadGoup)
        {
            if (thread)
                thread->join();
        }
        m_threadGoup.clear();
    }

private:
    std::list<std::shared_ptr<std::thread>> m_threadGoup; //处理任务的线程组
    SyncQueue<task> m_queue;
    atomic_bool m_running; //是否停止的标识
    std ::once_flag m_flag;
};

void tetsthpool()
{
    Cxx_threadpoo pool(2);
    

    std::thread th1([&pool]
                    {
        for(int i=0;i<10;i++)
        {
            auto thrd=this_thread::get_id();
            pool.AddTask([thrd]
            {
                cout<<"同步线程1的id"<<thrd<<endl;
            });
        } });

    std::thread th2([&pool]
                    {
        for(int i=0;i<10;i++)
        {
            auto thrd2=this_thread::get_id();
            pool.AddTask([thrd2]
            {
                cout<<"同步线程3的id"<<thrd2<<endl;
            });
        } }

    );
    this_thread::sleep_for(std::chrono::seconds(2));
    pool.Stop();

    th1.join();
    th2.join();
}

int main()
{
    tetsthpool();
}