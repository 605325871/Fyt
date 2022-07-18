# socke初识
>因为linux的一切皆文件的思想，所以对于网络编程来说同样也是利用一系列的网络函数对于文件进行交互修改

#  大小段字节序转换
>因为一些历史原因，本地机器一般通常用的都是小端模式模式存储，而网络用大端模式，所以一个进行大小端的转换，不能直接推测本地
 ```c
 #include <arpa/inet.h>

       uint32_t htonl(uint32_t hostlong); (port)

       uint16_t htons(uint16_t hostshort);(ip)

       uint32_t ntohl(uint32_t netlong); (ip)

       uint16_t ntohs(uint16_t netshort);(port)
```
> The htonl() function 将无符号整数 hostlong 从主机字节顺序转换为网络字节顺序.
The htons() function 将无符号短整数 hostshort 从主机字节顺序转换为网络字节顺序.
The ntohl() function 将无符号整数 netlong 从网络字节顺序转换为主机字节顺序.
The  ntohs() function 将无符号短整数 netshort 从网络字节顺序转换为主机字节顺序.
h代表host,n代表network，s代表short，l代表long。通常16位的IP端口号用s代表，而IP地址用l来代表。

# ip转换函数
>通常我们看的ip地址都是点分十进制的字符号表达，而通常的ip在网络中的传输为二进制数，因此就要进行转换

```c
const char *inet_ntop(int af, const void *src,
                             char *dst, socklen_t size);
```
>convert IPv4 and IPv6 addresses from binary to text 
```c
 int inet_pton(int af, const char *src, void *dst);

```
>convert IPv4 and IPv6 addresses from text to binary form



# socket
>sock就是一个创建一个操作网络io接口，用一个文件描述符去接受，可理解为是对缓冲区操作的钥匙
```c
int socket(int domain, int type, int protocol);
domain(): AF_INET,AF_INET6，AF_UNIX；
type(选用的协议): SOCK_STREAM (流式协议)，SOCK_DGRAM (报式协议)
protocol(选用协议的代表协议)：0 则如果时流式协议选用tcp-ip ，报式udp
```

# bind
> 将要操作的 地址结构（即ip+端口号）进行与socket所创建的文件描述符进行绑定，因为通过ip+端口可以确定出一个具体的进程进行通信
```c
int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
给socket绑定一个地址结构（ip +port)
addr ： {    使用前先初始化    
struct sockaddr_in addr;  
addr.sin_family = AF_INET;  
addr.sin_port = htons(); 
addr.sin_addr. s_addr= htonl(INADDR_ANY)//宏可以自行由机器在本地分配一个空闲的ip进行使用;
}
addR：（STRUCT SOCKADDR*）&addr
addrlen :szieof(addr);
```

# listen - listen for connections on a socket
>设置监听上限
```c
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
       int listen(int sockfd, int backlog);
```
sockfd: socket 函数返回值
backlog: 上限数值 ，最大128
# accept-accept a connection on a socket
```c
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```
阻塞等等待客户端连接，成功的话返回一个与客户端成功连接的socket文件

insockd：socket函数返回值
addr: 传出参数，成功与服务器建立的那个客户端的地址结构（IP+port）

# connect-initiate a connection on a socket
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
socketfd: socket 函数返回值
addr:传入参数，服务器的地址结构
addrlen：服务器的地址结构的大小·

****
例 完成一个大小写转换的c/s模型
tcp通信流程分析
>server：
       socket() 创建socket
       bind()    绑定服务器地址结构
       listen（） 设置监听上限
       accept（）  阻塞监听客户端
       read（）    读socket获取客户端数据
       小-大写     touper（）
       wirte（fd）  写回去
       close（）

>client：
       socket() 创建socket
       connect() 与服务器连接
       write（） 写数据到socket
       read()  读转换后的数据
       显示结果
       close（）

```c
//serve端
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>

int main(int argc, const char *argv[])
{
    // 创建用于监听的套节字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        perror("socket error");
        exit(1);
    }

    // 绑定
    struct sockaddr_in serv_addr;
    // init
    memset(&serv_addr, 0, sizeof(serv_addr));
    // bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;                // 地址族协议  ipv4
    serv_addr.sin_port = htons(9999);              // 本地端口， 需要转换为大端
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 0 是用本机的任意IP

    int ret = bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (ret == -1)
    {
        perror("bind error");
        exit(1);
    }

    // 设置监听
    ret = listen(lfd, 64);
    if (ret == -1)
    {
        perror("listen error");
        exit(1);
    }

    // 等待并接受连接请求
    struct sockaddr_in cline_addr;
    socklen_t clien_len = sizeof(cline_addr);
    int cfd = accept(lfd, (struct sockaddr *)&cline_addr, &clien_len);
    if (cfd == -1)
    {
        perror("accept error");
        exit(1);
    }

    char ipbuf[64];
    // int -> char*
    printf("cliient ip: %s, port: %d\n",
           inet_ntop(AF_INET, &cline_addr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)),
           ntohs(cline_addr.sin_port));

    // 通信
    while (1)
    {
        // 先接收数据
        char buf[1024] = {0};
        int len = read(cfd, buf, sizeof(buf));
        if (len == -1)
        {
            perror("read error");
            break;
        }
        else if (len > 0)
        {
            // 顺利读出了数据
            printf("read buf = %s\n", buf);
            // 小写 -》 大写
            for (int i = 0; i < len; ++i)
            {
                buf[i] = toupper(buf[i]);
            }
            printf(" -- toupper: %s\n", buf);

            // 数据发送给客户端
            write(cfd, buf, strlen(buf) + 1);
        }
        else if (len == 0)
        {
            printf("client disconnect ...\n");
            break;
        }
    }

    close(lfd);
    close(cfd);

    return 0;
}
```


```c
//client
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
/*
    int connect(int sockfd, const struct sockaddr *addr,
                   socklen_t addrlen);

*/
#define SERV_PORT 9527
void myerr(const char *str)
{
    perror(str);
    exit(1);
}

int main()
{
    int cfd, ret;
    int conter = 10;
    char buff[BUFSIZ];
    struct sockaddr_in clint_addr;
    socklen_t clint_len = sizeof(clint_addr);

    cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cfd == -1)
        myerr("socket error");
    clint_addr.sin_family = AF_INET;
    clint_addr.sin_port = htons(SERV_PORT);

    inet_pton(AF_INET, "127.0.0.1", &clint_addr.sin_addr.s_addr);
    if (connect(cfd, (struct sockaddr *)&clint_addr, clint_len) == -1)
        myerr("connect error");
    while (conter--)
    {
        write(cfd, "hello", 5);
        ret = read(cfd, buff, sizeof(buff));
        write(STDOUT_FILENO, buff, ret);
        sleep(1);
    }
    close(cfd);
    return 0;
}
```
