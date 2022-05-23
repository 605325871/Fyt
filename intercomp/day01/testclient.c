#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>

void myerr(const char *s)
{
    perror(s);
    exit(-1);
}
#define SERV_PORT 9556
int main()
{
    int sfd;
    int count = 10, ret;
    char buff[1024];
    struct sockaddr_in client_sockaddr;
    socklen_t client_sockaddr_len = sizeof(client_sockaddr);

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
        myerr("socket error");

    client_sockaddr.sin_family = AF_INET;
    client_sockaddr.sin_port = ntohs(SERV_PORT);
    inet_pton(AF_INET, "127.0.0.1", &client_sockaddr.sin_addr.s_addr);
    if (connect(sfd, (struct sockaddr *)&client_sockaddr, client_sockaddr_len) == -1)
        myerr("connect error");

    while (count--)
    {
        write(sfd, "hello", 6);
        ret = read(sfd, buff, sizeof(buff));
        write(STDOUT_FILENO, buff, ret);
        sleep(1);
    }
    close(sfd);
}