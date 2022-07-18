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
#include <sys/select.h>
#define SERV_PORT 9999
#define MAXLEN 1024
void perr_exit(const char *s)
{
    perror(s);
       exit(1);
}
int main()
{
    int lfd, cfd;
    socklen_t client_len;
    char buff[MAXLEN];
    struct sockaddr_in serv_addr, client_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
        perr_exit("socket error ");

    if (bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        perr_exit("bind error");

    if (listen(lfd, 128) == -1)
        perr_exit("listen error");

    int maxfd = lfd;
    fd_set redset;
    fd_set tmp;

    FD_ZERO(&redset);

    FD_SET(lfd, &redset); //将监听的lfd设置到检测的读集合中

    while (1)
    {
        tmp = redset; //创建副本
        int cnt = select(maxfd + 1, &tmp, NULL, NULL, NULL);//表示需要监听的文件描述符的最大值 + 1，其实本质是一个线性表为了监听到最后一个文件描述符所以+1；
        if (cnt < 0)
            perr_exit("select error");

        if (FD_ISSET(lfd, &tmp)) //判断是不是监听的lfd，是不是有新的客户端发起连接
        {
            cfd = accept(lfd, (struct sockaddr *)&client_addr, &client_len); //监听客户端，和发起请求的客户端建立连接
            if (cfd == -1)
                perr_exit("accept error");
    
            FD_SET(cfd, &redset);//加入到原始的数组中
            maxfd = cfd > maxfd ? cfd : maxfd;//判断 maxfd是不是当前最大上限
        }

        for (int j = lfd + 1; j <= maxfd; ++j)    ////// j从lfd下一个开始即可
        {
            if (FD_ISSET(j, &tmp)) // 接受数据
            {
                int ret = read(j, buff, sizeof(buff));
                if (ret == -1)
                    perr_exit("read error");
                else if (ret == 0)
                {
                    printf(" client close ----\n"); //该通信的客户端已经关闭
                    FD_CLR(j, &redset);             //将其从集合中移出去
                    close(j);
                }
                else
                {
                    for (int i = 0; i < ret; i++)
                        buff[i] = toupper(buff[i]);
                    ret = write(j, buff, ret);
                    if (ret == -1)
                        perr_exit("wirte error");
                    write(STDOUT_FILENO, buff, ret);
                }
            }
        }
    }
    return 0;
}