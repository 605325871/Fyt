#include "Tcpepoll1.2.hh"
Tcpepoll::Tcpepoll()
{
    signal(SIGPIPE, SIG_IGN);
    epfd = 0;
    m_fd = 0;
    evssize = sizeof(evs) / sizeof(evs[0]);
}
Tcpepoll::~Tcpepoll()
{
    if (m_fd > 0)
        close(m_fd);
    close(epfd);
}
inline void Tcpepoll::perr_exit(const char *s)
{
    perror(s);
    exit(-1);
}

int Tcpepoll::InitSocket(unsigned short port)
{

    signal(SIGPIPE, SIG_IGN);
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_port = port;
    serv_addr.sin_family = AF_INET;                // 地址族协议  ipv4
    serv_addr.sin_port = htons(serv_port);         // 本地端口， 需要转换为大端
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 0 是用本机的任意IP

    int opt = 1;
    setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); //端口复用

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

    ev.events = EPOLLIN | EPOLLET | EPOLLHUP;
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
        else if (nread == 0)
        {
            break;
        }
        else
        {
            if (errno == EAGAIN || errno == EINTR || errno == EWOULDBLOCK)
            {
                // printf("%s\n", errno == EAGAIN ? "EAGAIN" : "EWOULDBLOCK");
                continue;
            }
            std::cerr << errno << std::endl;
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
        if ((nwrite = write(_fd, p, count)) > 0)
        {
            p += nwrite;
            count -= nwrite;
        }
        else if (nwrite == 0)
        {
            break;
        }
        else
        {
            if (errno == EAGAIN || errno == EINTR || errno == EWOULDBLOCK)
            {
                // printf("%s\n", errno == EAGAIN ? "EAGAIN" : "EWOULDBLOCK");
                continue;
            }
            std::cerr << errno << std::endl;
        }
    }
    return size;
}
int Tcpepoll::sendMsg(std::string msg, int _fd)
{
    char *data = new char[msg.size() + 4];
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
    int x = 0;
    readn((char *)&len, 4, _fd);
    len = ntohl(len);

    char *buf = new char[len + 4];
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
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        for (int i = 0; i < num; ++i)
        {
            int fd = evs[i].data.fd;
            if (fd == m_fd)
            {
                int cfd = accept(fd, (struct sockaddr *)&client_addr, &client_len);
                char ipbuf[64];
                std::cout << " " << inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)) << " " << ntohs(client_addr.sin_port) << std::endl;

                fcntl(cfd, F_SETFL, O_NONBLOCK);
                ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
                ev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
            }
            else
            {
                std::string msg = recvMsg(fd);
                if (evs[i].events & (EPOLLOUT | EPOLLET | EPOLLRDHUP))
                {
                    int len = msg.size();
                    if (len == 0)
                    {
                        printf("client disconnect ...\n");

                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev); //先删除再关闭
                        close(fd);
                    }
                    else if (len > 0)
                    {
                        write(STDOUT_FILENO, msg.c_str(), len);
                        fcntl(fd, F_SETFL, O_NONBLOCK);
                        ev.events = EPOLLOUT | EPOLLET | EPOLLRDHUP;
                        ev.data.fd = fd;
                        epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
                    }
                    else if (len == -1)
                    {
                        perr_exit("recvmsg error");
                    }
                }
                else
                {
                    int len = sendMsg(msg, fd);
                    std::cout<<len<<std::endl;
                    if (len == 0)
                    {
                    }
                    else if (len > 0 && len == msg.size())
                    {
                        fcntl(fd, F_SETFL, O_NONBLOCK);
                        ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
                        ev.data.fd = fd;
                        epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
                    }
                }
            }
        }
    }
}