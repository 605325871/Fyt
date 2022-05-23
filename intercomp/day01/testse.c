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
#define myport 9556
int main()
{
    int sfd, cfd, ret;
    char buff[BUFSIZ];
    struct sockaddr_in serve_sockaddr, client_sockaddr;
    socklen_t serve_sockaddr_len = sizeof(serve_sockaddr), client_sockaddr_len = sizeof(client_sockaddr);

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
        myerr("socket error");

    serve_sockaddr.sin_family = AF_INET;
    serve_sockaddr.sin_port = htons(myport);
    serve_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sfd, (struct sockaddr *)&serve_sockaddr, serve_sockaddr_len) == -1)
        myerr("bind error");

    if (listen(sfd, 128) == -1)
        myerr("listen error");

    cfd = accept(sfd, (struct sockaddr *)&client_sockaddr, &client_sockaddr_len);
    if (cfd == -1)
        myerr("cfd error");

    while (1)
    {
        ret = read(cfd, buff, sizeof(buff));
        if (buff[0] == 'q')
            break;
        write(STDOUT_FILENO, buff, ret);
        for (int i = 0; i < ret; i++)
            buff[i] = toupper(buff[i]);

        write(cfd, buff, ret);
    }
    close(cfd);
    close(sfd);
    return 0;
}