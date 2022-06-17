#include"TcpSocket.hh"

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