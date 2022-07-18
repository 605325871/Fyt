#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include<stdlib.h>
#define MAXLINE 8192
#define SERV_PORT 9527

void perr_exit(char *s)
{
    perror(s);
    exit(1);
}

struct s_info
{
    struct sockaddr_in cliaddr;
    int connfd;
};

void *do_work(void *arg)
{
    int n, i;
    struct s_info *ts = (struct s_info *)arg;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];

    while (1)
    {   
        n = read(ts->connfd, buf, MAXLINE);
        if (n == 0)
        {
            printf(" the client %d closed \n", ts->connfd);
            break;
        }
        printf("receivd from %s,at port%d \n",
               inet_ntop(AF_INET, &(*ts).cliaddr.sin_addr, str, sizeof(str)),
               ntohs((*ts).cliaddr.sin_port));
        for (i = 0; i < n; i++)
            buf[i] = toupper(buf[i]);
        write(STDOUT_FILENO, buf, n);
        write(ts->connfd, buf, n);
    }
    close(ts->connfd);
    return NULL;
}

int main()
{
    struct sockaddr_in serv_addr, cliaddr;
    socklen_t cliaddr_len;
    int listenfd, connfd;
    pthread_t tid;
    struct s_info ts[256];

    int i = 0;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
        perr_exit("socket error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        perr_exit("bind error");

    if (listen(listenfd, 128) == -1)
        perr_exit("listen error");

    printf(" accepting client connext ...\n");

    while (1)
    {
        cliaddr_len = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
        if (connfd == -1)
            perr_exit("accpet error");
        ts[i].cliaddr = cliaddr;
        ts[i].connfd = connfd;

        pthread_create(&tid, NULL, do_work, (void *)&ts[i]);
        pthread_detach(tid);
        i++;
        if (i == 256)
            break;
    }
    return 0;
}