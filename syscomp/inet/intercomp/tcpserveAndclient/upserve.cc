#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include "TcpSocket.hh"
#define SERV_PORT 9527
class TcpServer
{
public:
    TcpServer();
    ~TcpServer();
    int setListen(unsigned short port);
    TcpSocket *acceptConn(struct sockaddr_in *addr=nullptr);

private:
    int m_fd; // 监听的套接字
};
TcpServer::TcpServer()
{
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
}
TcpServer::~TcpServer()
{

    close(m_fd);
}
int TcpServer::setListen(unsigned short port)
{
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(m_fd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1)
    {
        perror("bind");
        return -1;
    }
    ret = listen(m_fd, 128);
    if (ret == -1)
    {
        perror("listen");
        return -1;
    }
    return ret;
}

TcpSocket *TcpServer::acceptConn(struct sockaddr_in *addr)
{
    if (addr == nullptr)
    {
        return nullptr;
    }
    socklen_t addrlen = sizeof(struct sockaddr_in);
    int cfd = accept(m_fd, (struct sockaddr *)addr, &addrlen);
    if (cfd == -1)
    {
        perror("accept");
        return nullptr;
    }
    return new TcpSocket(cfd);
}

struct SockInfo
{
    TcpServer *s;
    TcpSocket *tcp;
    struct sockaddr_in addr;
};
void *working(void *arg)
{
    struct SockInfo *pinfo = static_cast<struct SockInfo *>(arg);
    char ip[INET_ADDRSTRLEN];
    printf("客户端的ip:%s,端口:%d\n", inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, ip, sizeof(ip))
     ,ntohs(pinfo->addr.sin_port));

     while (true)
     {
         printf("接受数据....\n");
         std::string msg = pinfo->tcp->recvMsg();
         if(!msg.empty())
         {
            std:: cout<<msg<< std::endl<<std::endl<<std::endl;
         }
         else
         {
             break;
         }
         delete pinfo->tcp;
         delete pinfo;
         return nullptr;
     }
     

}

int main()
{
    TcpServer s;
    s.setListen(9527);

    while (true)
    {
        SockInfo *info = new SockInfo;
        TcpSocket *tcp = s.acceptConn(&info->addr);
        if (tcp == nullptr)
        {
            std::cout << "重试...." << std::endl;
            continue;
        }
        pthread_t tid;
        info->s = &s;
        info->tcp = tcp;

        pthread_create(&tid, NULL, working, info);
        pthread_detach(tid);
    }
    return 0;
}