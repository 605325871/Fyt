#pragma oncee
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <sys/epoll.h>
#include "TcpSocket.hh"
#include <fcntl.h>
#include <sys/signal.h>
#include <list>
#include <memory>
struct myevent
{
    int m_event_status; //监听的事件
    int m_fd;           //监听的文件描述符

    void *arg;
    int len;
    myevent() = default;
    myevent(int _md) : m_fd(_md)
    {
        m_event_status = -1;
        len = 0;
    }
};

class Tcpepoll
{
private:
    int epfd;
    int m_fd;
    struct epoll_event ev;
    struct epoll_event evs[1024];
    int evssize;
    int serv_port{9527};
    std::list<int> clients_list;
    // std::unique_ptr<std::string> messg;
    std::string newmessg;
public:
    Tcpepoll(/* args */);
    ~Tcpepoll();
    int InitSocket(unsigned short port = 9527);
    void acceptConn();
    int sendMsg(std::string msg, int _fd);
    std::string recvMsg(int _fd);

private:
    int readn(char *buf, int size, int _fd);
    int writen(const char *msg, int size, int _fd);
    inline void perr_exit(const char *s);
};
