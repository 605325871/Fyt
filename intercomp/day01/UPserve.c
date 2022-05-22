#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERV_PORT 9527
/*
serve:
        int socket(int domain, int type, int protocol);
        int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
        int listen(int sockfd, int backlog);
        int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
*/
void myerr(const char *str)
{
    perror(str);
    exit(1);
}

int main()
{
    int lfd = 0, bid = 0, cfd = 0;
    int ret;
    struct sockaddr_in serve_addr, clit_addr;
    char buff[BUFSIZ], client_ip_[1024];
    socklen_t clit_addr_len = sizeof(clit_addr);

    serve_addr.sin_family = AF_INET;                //初始化为ipv4协议族
    serve_addr.sin_port = htons(SERV_PORT);         //将端口号从主机字节序转换成网络字节序
    serve_addr.sin_addr.s_addr = htonl(INADDR_ANY); //这个宏可以选用一个本机的

    lfd = socket(AF_INET, SOCK_STREAM, 0); //创建一个套接字 后续操作都依靠这个套接字
    if (lfd == -1)
        myerr("socket");
    bid = bind(lfd, (struct sockaddr *)&serve_addr, sizeof(serve_addr)); //将端口号与ip地址与套接字绑定
    if (bid == -1)
        myerr("bind");

    if (listen(lfd, 128) == -1)  //监听连接上限
        myerr("listen");

    cfd = accept(lfd, (struct sockaddr *)&clit_addr, &clit_addr_len);//阻塞等等待客户端连接，成功的话返回一个与客户端成功连接的socket文件
    if (cfd == -1)
        myerr("accept");

    printf("client ip : %s  , port :  %d",
           inet_ntop(AF_INET, &clit_addr.sin_addr.s_addr, client_ip_, sizeof(client_ip_)),
           ntohs(clit_addr.sin_port));
           
    while (1)
    {
        ret = read(cfd, buff, sizeof(buff));
        write(STDOUT_FILENO, buff, ret);
        for (int i = 0; i < ret; i++)
            buff[i] = toupper(buff[i]);
        write(cfd, buff, ret);
    }
    close(lfd);
    close(cfd);
    close(bid);
}