#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#define SERV_PORT 9999
void perr_exit(const char *s)
{
    perror(s);
    exit(-1);
}

void catch_child(int signum)
{
    while (waitpid(0, NULL, WNOHANG) > 0) ;
    return;
}
int main()
{
    int listenfd, connectfd;
    int ret, i;
    pid_t pid;
    char buff[BUFSIZ];
    char str[1024];
    socklen_t client_t;

    struct sockaddr_in serv_addr, client;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; //为serveaddr 结构体设置ipv4协议
    serv_addr.sin_port = htons(SERV_PORT);//将端口号 从本地小端 转换成大端
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//在本机中找出空闲的地址

    listenfd = socket(AF_INET, SOCK_STREAM, 0); // 利用listenfd 来操纵socket结构体的缓冲区
    if (listenfd == -1)
        perr_exit("socket error");

    if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) // 将缓冲区与地址结构进行绑定
        perr_exit("bind error");
    if (listen(listenfd, 128) == -1) //设置最大的监听上限
        perr_exit("listen error");

    while (1)
    {
        connectfd = accept(listenfd, (struct sockaddr *)&client, &client_t); //阻塞等待处理客户端发来请求

        pid = fork(); //将任务交给子进成管理
        if (pid < 0)
        {
            perr_exit("fork");
        }
        else if (pid == 0)
        {
            close(listenfd); //子进程关闭监听描述符
            while (1)
            {

                ret = read(connectfd, buff, sizeof(buff));
                if (ret == 0)
                {
                    printf(" the other side has been closed.\n");
                    break;
                }
                for (i = 0; i < ret; i++)
                    buff[i] = toupper(buff[i]);
                write(connectfd, buff, ret);

                write(STDOUT_FILENO, buff, ret);
            }
            close(connectfd);
            return 0;
        }
        else if (pid > 0)
        {
            struct sigaction act;       //父进程等待回收子进程
            act.sa_handler = catch_child;
            sigemptyset(&act.sa_mask);
            act.sa_flags = 0;

            ret = sigaction(SIGCHLD, &act, NULL);
            if (ret != 0)
            {
                perr_exit("sigaction");
            }

            close(connectfd); //关闭返回的新描述符
            continue;
        }
    }
}