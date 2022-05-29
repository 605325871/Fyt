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
int main(int argc, const char *argv[])
{
    // 创建用于监听的套节字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        perror("socket error");
        exit(1);
    }

    // 绑定
    struct sockaddr_in serv_addr;
    // init
    memset(&serv_addr, 0, sizeof(serv_addr));
    // bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;                // 地址族协议  ipv4
    serv_addr.sin_port = htons(9527);              // 本地端口， 需要转换为大端
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 0 是用本机的任意IP
    int opt;
    setsockopt(lfd,)
    int ret = bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (ret == -1)
    {
        perror("bind error");
        exit(1);
    }

    // 设置监听
    ret = listen(lfd, 64);
    if (ret == -1)
    {
        perror("listen error");
        exit(1);
    }

    int epfd = epoll_create(100); //创建一个epoll实例
    if (epfd == -1)
    {
        perror("e_create error");
        exit(1);
    }

    // /int epoll_ctl(int __epfd, int __op, int __fd, struct epoll_event *)

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = lfd;
    struct epoll_event evs[1024];
    int size = sizeof(evs) / sizeof(evs[0]);

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev) == -1)
    {
        perror("e_ctl error");
        exit(1);
    }
    while (1)
    {
        int num = epoll_wait(epfd, evs, size, -1);
        for (int i = 0; i < num; i++)
        {
            int fd = evs[i].data.fd;
            if (fd == lfd)
            {
                int cfd = accept(fd, NULL, NULL);
                ev.events = EPOLLIN;
                ev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
            }
            else
            {
                char buf[3] = {0};
                int len = read(fd, buf, sizeof(buf));
                if (len == -1)
                {
                    perror("read error");
                    break;
                }
                else if (len > 0)
                {
                    for (int i = 0; i < len; ++i)
                        buf[i] = toupper(buf[i]);

                    write(fd, buf, strlen(buf) + 1);
                    write(STDOUT_FILENO, buf, len);
                }
                else if (len == 0)
                {
                    printf("client disconnect ...\n");

                    epoll_ctl(epfd, EPOLL_CTL_DEL, lfd, &ev); //先删除再关闭
                    close(fd);
                }
            }
        }
    }

    close(lfd);

    return 0;
}