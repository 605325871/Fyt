#pragma once
#include <iostream>
#include <pthread.h>
#include"io_buf.hh"
#include <map>

// typedef std::map<int, io_buf *>  pool_t;
class buf_pool
{
public:
    //初始化单例对象
    static void init()
    {
        _instance = new buf_pool();
    }

    static buf_pool *instance()
    {
        //保证init方法在进程生命周期只执行一次
        pthread_once(&_once, init);

        return _instance; //返回
    }

    //从内存池中申请一块内存
    io_buf *alloc_buf(int N);
    io_buf *alloc_buf();

    // 重置了一个io_buf放回pool中
    void revert(io_buf *buffer);

    void make_io_buf_list(int cap ,int num);
private:
    buf_pool(/* args */);
    ~buf_pool();
    const buf_pool &operator=(const buf_pool &);

    //单例对象
    static buf_pool *_instance;

    //用于保证一个单创建单例的一个方法全局只初始化一次
    static pthread_once_t _once; // typedef int pthread_once_t

    //存放所有buf的map句柄
    std::map<int, io_buf *> _pool;

    //当前内存池的总体大小
    uint64_t _total_mem;

    static pthread_mutex_t _mutex;
};
