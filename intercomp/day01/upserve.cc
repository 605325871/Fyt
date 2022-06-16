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
#define SERV_PORT 9527
class TcpSocket
{
public:
    TcpSocket();
    TcpSocket(int socket);
    ~TcpSocket();
    int connectToHost(std::string ip, unsigned short port);
    int sendMsg(std::string msg);
    std::string recvMsg();

private:
    int readn(char *buf, int size);
    int writen(const char *msg, int size);

private:
    int m_fd; // 通信的套接字
};

TcpSocket::TcpSocket()
{
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
}
TcpSocket::TcpSocket(int socket)
{
    m_fd = socket;
}

TcpSocket::~TcpSocket()
{
    if (m_fd > 0)
    {
        close(m_fd);
    }
}
int TcpSocket::connectToHost(std::string ip, unsigned short port)
{
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);

    inet_pton(AF_INET, ip.data(), &saddr.sin_addr.s_addr);
    int ret = connect(m_fd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1)
    {
        perror("connect error");
        exit(-1);
    }
    return ret;
}
int TcpSocket::sendMsg(std::string msg)
{
    char *data = new char[msg.size() + 1];
    uint32_t biglen = htonl(msg.size());
    memcpy(data, &biglen, 4); //包头四字节存长度+数据长度
    memcpy(data + 4, msg.data(), msg.size());

    int ret = writen(data, msg.size() + 4);
    return ret;
}

int TcpSocket::writen(const char *msg, int size)
{
    int left = size;
    int nwrite = 0;
    const char *p = msg;

    while (left > 0)
    {
        if ((nwrite = write(m_fd, msg, left)) > 0)
        {
            p += nwrite;
            left -= nwrite;
        }
        else if (nwrite == -1)
        {
            return -1;
        }
    }
    return size;
}

int TcpSocket::readn(char *buf, int size)
{
    int nread = 0;
    int count = size;
    char *p = buf;
    while (count > 0)
    {
        if ((nread = read(m_fd, p, count)) > 0)
        {
            p += nread;
            count -= nread;
        }
        else if (nread == -1)
        {
            return -1;
        }
    }
    return size;
}

std::string TcpSocket::recvMsg()
{
    int len = 0;
    readn((char *)&len, 4);
    len = ntohl(len);

    //根据读出的长度从新分配内存
    char *buf = new char[len + 1];
    int ret = readn(buf, len);
    if (ret != len)
    {
        return std::string();
    }
    buf[len] = '\0';
    std::string retstr(buf);
    delete[] buf;
    return retstr;
}

class TcpServer
{
public:
    TcpServer();
    ~TcpServer();
    int setListen(unsigned short port);
    TcpSocket *acceptConn(struct sockaddr_in *addr = nullptr);

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

TcpSocket *TcpServer::acceptConn(struct sockaddr_in *addr = nullptr)
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
int main()
{
    TcpSocket tcsocket;
    int ret = tcsocket.connectToHost("127.0.0.1", 9527);
    if (ret == -1)
        return -1;
    
}