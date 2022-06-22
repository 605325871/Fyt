#pragma once
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <sys/epoll.h>
#include "TcpSocket.hh"
#define EXPORT 9527
class Tcpepoll
{
private:
    int epfd;
    int m_fd;
    struct epoll_event ev;
    struct epoll_event evs[1024];
    int evssize;

public:
    Tcpepoll(/* args */);
    ~Tcpepoll();
    void perr_exit(const char *s);
    int InitSocket(unsigned short port = EXPORT);
    void acceptConn();
    int sendMsg(std::string msg, int _fd);
    std::string recvMsg(int _fd);

private:
    int readn(char *buf, int size, int _fd);
    int writen(const char *msg, int size, int _fd);
};
