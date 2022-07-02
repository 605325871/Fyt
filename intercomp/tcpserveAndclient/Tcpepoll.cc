#include "Tcpepoll.hh"
Tcpepoll::Tcpepoll()
{
    epfd = 0;
    m_fd = 0;
    evssize = sizeof(evs) / sizeof(evs[0]);
}
Tcpepoll::~Tcpepoll()
{
    if(m_fd>0)
    close(m_fd);
}
void Tcpepoll::perr_exit(const char *s)
{
    perror(s);
    exit(-1);
}
int Tcpepoll::InitSocket(unsigned short port)
{

    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;                // 地址族协议  ipv4
    serv_addr.sin_port = htons(port);              // 本地端口， 需要转换为大端
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 0 是用本机的任意IP

    int ret = bind(m_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (ret == -1)
        perr_exit("bind error");

    // 设置监听
    ret = listen(m_fd, 128);
    if (ret == -1)
        perr_exit("listen error");

    epfd = epoll_create(100);
    if (epfd == -1)
        perr_exit("epoll_creat error");
    ev.events = EPOLLIN;
    ev.data.fd = m_fd;
    evssize = sizeof(evs) / sizeof(evs[0]);
    if ((ret = epoll_ctl(epfd, EPOLL_CTL_ADD, m_fd, &ev)) == -1)
        perr_exit("epoll_ctl error");
    return ret;
}
int Tcpepoll::readn(char *buf, int size, int _fd)
{
    int nread = 0;
    int count = size;
    char *p = buf;

    while (count > 0)
    {
        if ((nread = read(_fd, p, count)) > 0)
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

int Tcpepoll::writen(const char *msg, int size, int _fd)
{
    int nwrite = 0;
    int count = size;
    const char *p = msg;
    while (count > 0)
    {
        if ((nwrite = write(_fd, msg, count)) > 0)
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
int Tcpepoll::sendMsg(std::string msg, int _fd)
{
    char *data = new char[msg.size() + 1];
    int biglen = htonl(msg.size());
    memcpy(data, &biglen, 4);
    memcpy(data + 4, msg.data(), msg.size());

    int ret = writen(data, msg.size() + 4, _fd);
    delete[] data;
    return ret;
}
std::string Tcpepoll::recvMsg(int _fd)
{
    // 先读包头
    int len = 0;
    int x=0;
    readn((char *)&len, 4,_fd);
    len = ntohl(len);

    char *buf = new char[len + 1];
    int ret = readn(buf, len, _fd);
    if (ret != len)
    {
        return std::string();
    }
    buf[len] = '\0';
    std::string retstr(buf);
    delete[] buf;
    return retstr;
}

void Tcpepoll::acceptConn()
{
    while (true)
    {
        int num = epoll_wait(epfd, evs, evssize, -1);
        for (int i = 0; i < num; ++i)
        {
            int fd = evs[i].data.fd;
            if (fd == m_fd)
            {
                int cfd = accept(fd, NULL, NULL);
                ev.events = EPOLLIN;
                ev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
            }
            else
            {
                std::string msg = recvMsg(fd);
                int len = msg.size();
                if (len == 0)
                {
                    printf("client disconnect ...\n");

                    epoll_ctl(epfd, EPOLL_CTL_DEL, m_fd, &ev); //先删除再关闭
                    close(m_fd);
                }
                else if (len > 0)
                {
                    sendMsg(msg, fd);
                }
                else if (len == -1)
                {
                    perr_exit("recvmsg error");
                    
                }
            }
        }
    }
}