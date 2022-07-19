#pragma once
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>
class tcp_server
{
private:
    int _socketfd; //套接字
    struct sockaddr_in _conn_addr;
    socklen_t _addrlen;
    inline void perr_exit(const char *s);

public:
    tcp_server(const char *ip, uint16_t port);
    ~tcp_server();
    void do_accept();
};

