# epoll

# epoll_creat
**NAME**
       epoll_create, epoll_create1 - **open an epoll file descriptor**
```c
       #include <sys/epoll.h>

       int epoll_create(int size);
```
>size : 创建红黑树的监听节点数量。
返回值：指向新创建的红黑树的根节点的fd
失败返回-1

# epoll_ctl
**NAME**
       epoll_ctl - **control interface for an epoll file descriptor**

```c
       #include <sys/epoll.h>

       int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
```
> epfd:监听的指向红黑树根节点的fd，
> events:修改监听事件，（EPOLL_CTL_ADDEPOLL_CTL_MOD，EPOLL_CTL_DEL）
> 监听的文件描述符 
```c
>struct epoll_event *event结构体
           struct epoll_event {
               uint32_t     events;          /* Epoll events */
               epoll_data_t data;            /* User data variable */
           };
```
其中data为一个联合体
```c
>   typedef union epoll_data {
               void        *ptr;
               int          fd;
               uint32_t     u32;
               uint64_t     u64;
           } epoll_data_t;
```
# epoll_Wait
**NAME**
       epoll_wait,  epoll_pwait  -  **wait for an I/O event on an epoll file de‐**
       scriptor
```c
       #include <sys/epoll.h>

       int epoll_wait(int epfd, struct epoll_event *events,
                      int maxevents, int timeout);
```
>epfd:监听的指向红黑树根节点的fd，
>events[数组]:是一个传出参数，得到由满足监听条件的fd事件
>size:events数组的大小
>timeout： -1 : 阻塞等待  ，  0不阻塞   ，> 0  超时时间

>返回值： >0 满足监听事件的总数,可以作为循环上限
         =0  没有满足条件的事件发生
         <0  错误，返回errornumber







