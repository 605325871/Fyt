# multiple io transfer --select
>这种方式可以同时监测多个文件描述符并且这个过程是阻塞的，一旦检测到有文件描述符就绪（ 可以读数据或者可以写数据）程序的阻塞就会被解除，之后就可以基于这些（一个或多个）就绪的文件描述符进行通信了

# select

>委托内核帮助我们检测若干个文件描述符的状态
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

>readfds：文件描述符的集合，内核只检测这个集合中文件描述符对应的读缓冲区这样才知道通过哪个文件描述符接收数据

>writefds：文件描述符的集合，内核只检测这个集合中文件描述符对应的写缓冲区,如果不需要使用这个参数可以指定为 NULL

>exceptfds：文件描述符的集合，内核检测集合中文件描述符是否有异常状态,如果不需要使用这个参数可以指定为 NULL

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
```
```c
int  FD_ISSET(int fd, fd_set *set);// 判断文件描述符fd是否在set集合中 == 读一下fd对应的标志位到底是0还是1
```
```c
void FD_SET(int fd, fd_set *set);// 将文件描述符fd添加到set集合中 == 将fd对应的标志位设置为1
```
```c
void FD_ZERO(fd_set *set);     //将set集合中, 所有文件文件描述符对应的标志位设置为0, 集合中没有添加任何文件描述符
```
例子使用流程
>前面准备工作不变，socket()->bind()->listen()

```c
int lfd, cfd;
    socklen_t client_len;
    char buff[MAXLEN];
    struct sockaddr_in serv_addr, client_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
        perr_exit("socket error ");

    if (bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        perr_exit("bind error");

    if (listen(lfd, 128) == -1)
        perr_exit("listen error");
```

>要设置一个遍历最大文件描述符的变量，进行进行更新，遍历的最大上限,初始大小设为socked所监听的文件描述符大小即可

```c
int maxfd = lfd;
```
>创建两个fd_set 型数组，用于存储和遍历，因为在遍历时可能会对里面参数作出改变所以创建一个用来当另一个副本,完成初始化和监听事件的添加

```c
FD_ZERO(&redset);
FD_SET(lfd, &redset); //将监听的lfd设置到检测的读集合中
```
下面的处理已在后面编出完整注释
```c
 while (1)
    {
        tmp = redset; //创建副本
        int cnt = select(maxfd + 1, &tmp, NULL, NULL, NULL);//表示需要监听的文件描述符的最大值 + 1，其实本质是一个线性表为了监听到最后一个文件描述符所以+1；
        if (cnt < 0)
            perr_exit("select error");

        if (FD_ISSET(lfd, &tmp)) //判断是不是监听的lfd，是不是有新的客户端发起连接
        {
            cfd = accept(lfd, (struct sockaddr *)&client_addr, &client_len); //监听客户端，和发起请求的客户端建立连接
            if (cfd == -1)
                perr_exit("accept error");
    
            FD_SET(cfd, &redset);//加入到原始的数组中
            maxfd = cfd > maxfd ? cfd : maxfd;//判断 maxfd是不是当前最大上限
        }

        for (int j = lfd + 1; j <= maxfd; ++j)    ////// j从lfd下一个开始即可
        {
            if (FD_ISSET(j, &tmp)) // 接受数据
            {
                int ret = read(j, buff, sizeof(buff));
                if (ret == -1)
                    perr_exit("read error");
                else if (ret == 0)
                {
                    printf(" client close ----\n"); //该通信的客户端已经关闭
                    FD_CLR(j, &redset);             //将其从集合中移出去
                    close(j);
                }
                else
                {
                    for (int i = 0; i < ret; i++)
                        buff[i] = toupper(buff[i]);
                    ret = write(j, buff, ret);
                    if (ret == -1)
                        perr_exit("wirte error");
                    write(STDOUT_FILENO, buff, ret);
                }
            }
        }
    }
```

