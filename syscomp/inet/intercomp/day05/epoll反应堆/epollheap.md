# epollheap

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

et模式+ 非阻塞+void* ptr

原模式：socket，bind，listen，--epoll_creat() 创建监听 红黑树 --,返回epfd,--epoll_ctl()添加监听事件，
while(1)
{  
    epoll_wait()监听--，返回监听满足数组，--判断返回数组元素--，lfd满足，--accpet，cfd满足，read,转换--wirte回去
}

现模式：socket，bind，listen，--epoll_creat() 创建监听 红黑树 --,返回epfd,--epoll_ctl()添加监听事件，
while(1)
{  
    epoll_wait()监听--，返回监听满足数组，--判断返回数组元素--，lfd满足，--accpet，cfd满足，read,转换，
    cfd从红黑树上摘下来--epollout--回调函数--epoll_ctl()  --epoll_ctl重新放到红黑树上监听写事件，等待epoll_create()f返回--说明cfd可写 --write回去

}

