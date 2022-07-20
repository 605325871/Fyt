#include "../include/tcp_server.hh"
#include <signal.h>
#include "../include/reator_bufer.hh"
inline void tcp_server::perr_exit(const char *s)
{
    perror(s);
    exit(-1);
}

void accept_callback(event_loop *loop, int fd, void *args)
{
    tcp_server *server = (tcp_server *)args;
    server->do_accept();
}

tcp_server::tcp_server(event_loop *loop, const char *ip, uint16_t port)
{
    //忽略SIGEPIP,SIGEHUB
    if (signal(SIGHUP, SIG_IGN) == SIG_ERR)
        perr_exit("SIGHUP err");

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) //防止对端关闭奔溃
        perr_exit("SIGPIPE err");

    if ((_socketfd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0)) < 0)
        perr_exit("socket error");

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_aton(ip, &server_addr.sin_addr);
    _addrlen = sizeof(_conn_addr);

    int opt = 1;
    setsockopt(_socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); //端口复用

    if ((bind(_socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr))) < 0)
        perr_exit("bind error");

    if ((listen(_socketfd, 128)) < 0)
        perr_exit("listen error");

    //将loop加入到
    _loop = loop;

    //注册_socketfd事件
    _loop->add_epoll_event(_socketfd, accept_callback, EPOLLIN, this);
}
tcp_server::~tcp_server()
{
}

void tcp_server::do_accept()
{
    int connfd;

    while (true)
    {
        connfd = accept(_socketfd, (struct sockaddr *)&_conn_addr, &_addrlen);
        if (connfd == -1)
        {
            if (errno == EINTR)
            {
                perror("EINTER ERROR");
                continue;
            }
            else if (errno == EAGAIN)
            {
                perror("EAGAIN ERROR");
                break;
            }
            else if (errno == EMFILE) //资源不够
            {
                perr_exit("EMFILE ERROR");
            }
            else
            {
                std::cerr << "err: " << std::endl;
                exit(-1);
            }
        }
        else
        {
            //添加心跳机制
            //添加消息队列

            //回显
            input_buffer ibf;
            ouput_buffer obf;
            int ret;
            do
            {
                char *msg = nullptr;
                int msg_len = 0;
                ret = ibf.read_data(connfd);
                if (ret == -1)
                {
                    std::cerr << "read_data err" << std::endl;
                    exit(-1);
                    break;
                }

                msg_len = ibf.legth();
                msg = new char[msg_len];
                bzero(msg, msg_len);
                memcpy(msg, ibf.data(), msg_len);

                ibf.pop(msg_len);
                ibf.adjust();

                printf("recv data =%s\n", msg);

                //将数据写道out——buf中
                obf.send_data(msg, msg_len);
                while (obf.legth())
                {
                    int writeed = obf.write2fd(connfd);
                    if (writeed == -1)
                    {
                        std::cerr << "read_data err" << std::endl;
                        exit(-1);
                    }
                    else if (writeed == 0) // writedd为0说明还写未写入
                    {
                        //当前不能写等待下次机会
                        continue;
                    }
                }

                delete msg;
            } while (ret != 0);
            close(connfd);
            //将数据读到io_buf中
        }
    }
}