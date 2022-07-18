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

# epoll的三种工作模式
边沿触发
水平触发
边沿非阻塞出发


>一、ET模式（边沿触发）的文件描述符(fd)：

         当epoll_wait检测到fd上有事件发生并将此事件通知应用程序后，应用程序必须立即处理该事件，因为后续的epoll_wait调用将不再向应用程序通知这一事件。

         epoll_wait只有在客户端第一次发数据是才会返回,以后即使缓冲区里还有数据，也不会返回了。epoll_wait是否返回，是看客户端是否发数据，客户端发数据了就会返回，且只返回一次。

         eg：客户端发送数据，I/O函数只会提醒一次服务端fd上有数据，以后将不会再提醒

所以要求服务端必须一次把数据读完--->循环读数据 (读完数据后，可能会阻塞)  --->将描述符设置成非阻塞模式

二、LT模式（水平触发）的文件描述符(fd)：

         当epoll_wait检测到fd上有事件发生并将此事件通知应用程序后，应用程序可以不立即处理该事件，这样，当应用程序下一次调用epoll_wait时，epoll_wait还会再次向应用程序通知此事件，直到此事件被处理。

eg：客户端发送数据，I/O函数会提醒描述符fd有数据---->recv读数据，若一次没有读完，I/O函数会一直提醒服务端fd上有数据，直到recv缓冲区里的数据读完