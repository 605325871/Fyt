#include "../include/buf_pool.hh"

//定义一些内存的刻度
enum MEM_CAP
{
    m4k = 4096,
    m16k = 16384,
    m64k = 65535,
    m1M = 1048576
};

#define MEM_LIMIT (5U * 1024 * 1024)
//单例对象
buf_pool *buf_pool::_instance = NULL;

pthread_mutex_t buf_pool::_mutex = PTHREAD_MUTEX_INITIALIZER; //初始化锁

//用于保证一个单创建单例的一个方法全局只初始化一次
pthread_once_t buf_pool::_once = PTHREAD_ONCE_INIT; // typedef int pthread_once_t

buf_pool::buf_pool(/* args */)
{
    make_io_buf_list(m4k, 5000);
    make_io_buf_list(m16k, 1000);
    make_io_buf_list(m64k, 500);
    make_io_buf_list(m1M, 500);
}

void buf_pool::make_io_buf_list(int cap, int num)
{

    //链表的头指针
    io_buf *prev = nullptr;
    _pool[cap] = new io_buf(cap);
    if (_pool[cap] == nullptr)
    {
        std::cerr << "new error " << std::endl;
        exit(-1);
    }
    prev = _pool[cap];
    for (int i = 1; i < num; ++i)
    {
        prev->next = new io_buf(cap);
        if (prev->next == nullptr)
        {
            std::cerr << "new error " << std::endl;
            exit(-1);
        }
        prev = prev->next;
    }
    _total_mem += 4 * 5000;
    if (_total_mem > MEM_LIMIT)
        return;

    //开16k..，
}
buf_pool::~buf_pool()
{
}
const buf_pool &buf_pool::operator=(const buf_pool &)
{
}
io_buf *buf_pool::alloc_buf(int N)
{
    int index; //找到N最接近的一个刻度链表，返回一个iobuf
    if (N <= m4k)
        index = m4k;
    else if (N > m4k && N <= m16k)
        index = m16k;
    else if (index > m16k && index <= m64k)
    {
        index = m64k;
    }
    else
    {
        index == NULL;
    }

    // 2 如果index有内存，需要额外的申请内存
    pthread_mutex_lock(&_mutex);
    if (_pool[index] == NULL)
    {
        //如果链表为空，需要新申请inex大小的io——buf
        if (_total_mem + index / 1024 >= MEM_LIMIT) //如果超过最大限制
        {
            std::cerr << "byond MEM_LIMIT " << std::endl;
            exit(-1);
        }
        io_buf *new_buf = new io_buf(index);
        if (new_buf == nullptr)
        {
            std::cerr << " NEW ERROR 91" << std::endl;
            exit(-1);
        }
        _total_mem += index / 1024;
        pthread_mutex_unlock(&_mutex);
        return new_buf;
    }

    // 3如果index有内存，从pool中拆除一块内存返回
    io_buf *target = _pool[index];
    _pool[index] = target->next;

    pthread_mutex_unlock(&_mutex);
    target->next = nullptr;

    return target;
}
io_buf *buf_pool::alloc_buf()
{
    return alloc_buf(m4k);
}

// 重置了一个io_buf放回pool中
void buf_pool::revert(io_buf *buffer)
{
    //将一个buff放回
    // index是属于pool中的呢个链表
    int index = buffer->_capcity;
    buffer->_length = 0;
    buffer->_head = 0;

    pthread_mutex_lock(&_mutex);

    if (_pool.find(index) == _pool.end())
    {
        std::cerr << " NEW ERROR126 " << std::endl;
        exit(-1);
    }

    buffer->next = _pool[index];
    _pool[index] = buffer;

    pthread_mutex_unlock(&_mutex);
}
