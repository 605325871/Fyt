# multiple io transfer
>这种方式可以同时监测多个文件描述符并且这个过程是阻塞的，一旦检测到有文件描述符就绪（ 可以读数据或者可以写数据）程序的阻塞就会被解除，之后就可以基于这些（一个或多个）就绪的文件描述符进行通信了

# select
select
>委托内核帮助我们检测若干个文件描述符的状态,
若是读缓冲区里读缓冲区里面有数据，读缓冲文件描述符准备就绪
```c
#include <sys/select.h>
struct timeval {
    time_t      tv_sec;         /* seconds */
    suseconds_t tv_usec;          //需要进行初始化，否则就是一个随机值         /* microseconds */
};

int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval * timeout);
```
>nfds：委托内核检测的这三个集合中最大的文件描述符 + 1,内核需要线性遍历这些集合中的文件描述符，这个值是==循环结束的条件==在 Window 中这个参数是无效的，指定为 - 1 即可
readfds：文件描述符的集合，内核只检测这个集合中文件描述符对应的读缓冲区这样才知道通过哪个文件描述符接收数据
writefds：文件描述符的集合，内核只检测这个集合中文件描述符对应的写缓冲区,如果不需要使用这个参数可以指定为 NULL
exceptfds：文件描述符的集合，内核检测集合中文件描述符是否有异常状态,如果不需要使用这个参数可以指定为 NULL

>timeout：超时时长，用来强制解除 select () 函数的阻塞的
NULL：函数检测不到就绪的文件描述符会一直阻塞。
等待固定时长（秒）：函数检测不到就绪的文件描述符，在指定时长之后强制解除阻塞，函数返回 0
不等待：函数不会阻塞，直接将该参数对应的结构体初始化为 0 即可。
函数返回值：
大于 0：成功，返回集合中已就绪的文件描述符的总个数
等于 - 1：函数调用失败
等于 0：超时，没有检测到就绪的文件描述符

```c     
void FD_CLR(int fd, fd_set *set);// 将文件描述符fd从set集合中删除 == 将fd对应的标志位设置为0   
int  FD_ISSET(int fd, fd_set *set);// 判断文件描述符fd是否在set集合中 == 读一下fd对应的标志位到底是0还是1
void FD_SET(int fd, fd_set *set);// 将文件描述符fd添加到set集合中 == 将fd对应的标志位设置为1
void FD_ZERO(fd_set *set);     //将set集合中, 所有文件文件描述符对应的标志位设置为0, 集合中没有添加任何文件描述符

```