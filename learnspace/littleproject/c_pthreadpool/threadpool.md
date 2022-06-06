
需要一个处理任务的结构体，与管理线程池属性的结构体
```c
// 任务结构体
typedef struct Task
{
    void (*function)(void *arg);
    void *arg;
} Task;
// 线程池结构体
struct ThreadPool
{
    // 任务队列
    Task *taskQ;
    int queueCapacity; // 容量
    int queueSize;     // 当前任务个数
    int queueFront;    // 队头 -> 取数据
    int queueRear;     // 队尾 -> 放数据

    pthread_t managerID;       // 管理者线程ID
    pthread_t *threadIDs;      // 工作的线程ID
    int minNum;                // 最小线程数量
    int maxNum;                // 最大线程数量
    int busyNum;               // 忙的线程的个数
    int liveNum;               // 存活的线程的个数
    int exitNum;               // 要销毁的线程个数
    pthread_mutex_t mutexPool; // 锁整个的线程池
    pthread_mutex_t mutexBusy; // 锁busyNum变量
    pthread_cond_t notFull;    // 任务队列是不是满了
    pthread_cond_t notEmpty;   // 任务队列是不是空了

    int shutdown; // 是不是要销毁线程池, 销毁为1, 不销毁为0
};
```
模块分析
1.main()
    创建线程池
    向线程池中添加任务，借助回调函数处理任务
    销毁线程池

2.pthreadpool_create()
    创建线程池结构体指针
    初始化线程池结构体（n个变量）
    创建N个任务线程
    创建1个管理者线程
    失败时销毁所有创建空间