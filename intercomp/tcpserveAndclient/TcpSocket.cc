#include "TcpSocket.hh"
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
        close(m_fd);
}
void TcpSocket::perr_exit(const char *s)
{
    perror(s);
    std::cout << __LINE__ << std::endl;
    exit(-1);
}
int TcpSocket::connectToHost(std::string ip, unsigned short port)
{
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);

    inet_pton(AF_INET, ip.data(), &saddr.sin_addr.s_addr);
    int ret = connect(m_fd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1)
        perr_exit("connect error");
    return ret;
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
            perr_exit("nread error");
        }
    }
    return size;
}

int TcpSocket::writen(const char *msg, int size)
{
    int nwrite = 0;
    int count = size;
    const char *p = msg;
    while (count > 0)
    {
        if ((nwrite = write(m_fd, msg, count)) > 0)
        {
            p += nwrite;
            count -= nwrite;
        }
        else if (nwrite == -1)
        {
            perr_exit("nwrite error");
        }
    }
    return size;
}
int TcpSocket::sendMsg(std::string msg)
{
    char *data = new char[msg.size() + 1];
    int biglen = htonl(msg.size());
    memcpy(data, &biglen, 4);
    memcpy(data + 4, msg.data(), msg.size());

    int ret = writen(data, msg.size() + 4);
    delete[] data;
    return ret;
}
std::string TcpSocket::recvMsg()
{
    // 先读包头
    int len = 0;
    readn((char *)&len, 4);
    len = ntohl(len);

    char* buf = new char[len + 1];
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
