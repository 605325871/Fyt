

#  convert values between host and networkbyte order
 ```cpp
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
```c
int socket(int domain, int type, int protocol);
domain(): AF_INET,AF_INET6，AF_UNIX；
type(选用的协议): SOCK_STREAM (流式协议)，SOCK_DGRAM (报式协议)
protocol(选用协议的代表协议)：0 则如果时流式协议选用tcp-ip ，报式udp
```

# bind
```c
int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
给socket绑定一个地址结构（ip +port)
addr ： {    使用前先初始化    
struct sockaddr_in addr;  
addr.sin_family = AF_INET;  
addr.sin_port = htons(); 
addr.sin_addr. s_addr= htonl(INADDR_ANY)//;
}
addR：（STRUCT SOCKADDR*）&addr
addrlen :szieof(addr);
```

# listen - listen for connections on a socket
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


例 完成一个大小写转换的c/s模型
tcp通信流程分析
server：
       socket() 创建socket
       bind()    绑定服务器地址结构
       listen（） 设置监听上限
       accept（）  阻塞监听客户端
       read（）    读socket获取客户端数据
       小-大写     touper（）
       wirte（fd）  写回去
       close（）
client：
       socket() 创建socket
       connect() 与服务器连接
       write（） 写数据到socket
       read()  读转换后的数据
       显示结果
       close（）

