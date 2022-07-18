#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
    int connect(int sockfd, const struct sockaddr *addr,
                   socklen_t addrlen);

*/
#define SERV_PORT 9527
void myerr(const char *str)
{
    perror(str);
    exit(1);
}

int main()
{
    int cfd, ret;
    int conter = 10;
    char buff[BUFSIZ];
    struct sockaddr_in clint_addr;
    socklen_t clint_len = sizeof(clint_addr);

    cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cfd == -1)
        myerr("socket error");
    clint_addr.sin_family = AF_INET;
    clint_addr.sin_port = htons(SERV_PORT);

    inet_pton(AF_INET, "127.0.0.1", &clint_addr.sin_addr.s_addr);
    if (connect(cfd, (struct sockaddr *)&clint_addr, clint_len) == -1)
        myerr("connect error");
    int fd1 = open("1.txt",O_RDONLY);
    int legth=0; 
    char tmp[100];
    while ((legth = read(fd1,tmp,sizeof(tmp)))>0)
    {
        write(cfd, tmp, legth);
        
        usleep(300);
    }
    close(cfd);
    return 0;
}