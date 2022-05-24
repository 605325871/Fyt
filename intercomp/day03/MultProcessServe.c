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
    while (waitpid(0, NULL, WNOHANG) > 0)
        ;
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
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
        perr_exit("socket error");

    if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        perr_exit("bind error");
    if (listen(listenfd, 128) == -1)
        perr_exit("listen error");
    printf("received from %s at port %d\n",
           inet_ntop(AF_INET, &client.sin_addr, str, sizeof(str)),
           ntohs(client.sin_port));

    while (1)
    {
        connectfd = accept(listenfd, (struct sockaddr *)&client, &client_t);

        pid = fork();
        if (pid < 0)
        {
            perr_exit("fork");
        }
        else if (pid == 0)
        {
            close(listenfd);
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
            struct sigaction act;
            act.sa_handler = catch_child;
            sigemptyset(&act.sa_mask);
            act.sa_flags = 0;

            ret = sigaction(SIGCHLD, &act, NULL);
            if (ret != 0)
            {
                perr_exit("sigaction");
            }

            close(connectfd);
            continue;
        }
    }
}