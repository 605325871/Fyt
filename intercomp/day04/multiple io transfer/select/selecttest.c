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

#define serv_port 9999
void perr_exit(const char *s)
{
    perror(s);
    exit(1);
}
int main()
{
    socklen_t client_addr_len;
    int lfd, cfd;
    char buff[BUFSIZ];
    struct sockaddr_in serv_add, client_addr;
    serv_add.sin_family = AF_INET;
    serv_add.sin_port = htons(serv_port);
    serv_add.sin_addr.s_addr = htonl(INADDR_ANY);

    lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
        perr_exit("socket error ");
    if (bind(lfd, (struct sockaddr *)&serv_add, sizeof(serv_add)) == -1)
        perr_exit("bind error ");
    if (listen(lfd, 128) == -1)
        perr_exit("listen error ");

    int maxfd = lfd;
    fd_set readset, temp;

    FD_ZERO(&readset);
    FD_SET(lfd, &readset);

    while (1)
    {
        temp = readset;
        client_addr_len = sizeof(client_addr);
        int cnt = select(maxfd + 1, &temp, NULL, NULL, NULL);
        if (cnt < 0)
            perr_exit("SELSECT ERROR");

        if (FD_ISSET(lfd, &temp))
        {
            cfd = accept(lfd, (struct sockaddr *)&client_addr, &client_addr_len);
            if (cfd == -1)
                perr_exit("accept error ");
            char ip[INET_ADDRSTRLEN];
            printf("new client ip :%s, port :%d\n",
                   inet_ntop(AF_INET, &client_addr.sin_addr, ip,
                             sizeof(ip)),
                   ntohs(client_addr.sin_port));
            FD_SET(cfd, &readset);
            maxfd = cfd > maxfd ? cfd : maxfd;
        }

        for (int j = lfd + 1; j <= maxfd; j++)
        {
            if (FD_ISSET(j, &temp))
            {
                int ret = read(j, buff, sizeof(buff));
                if (ret == 0)
                {
                    printf(" client close ----\n");
                    FD_CLR(j, &readset);
                    close(j);
                }
                else if (ret > 0)
                {
                    for (int i = 0; i < ret; i++)
                        buff[i] = toupper(buff[i]);
                    ret = write(j, buff, ret);
                    if (ret == -1)
                        perr_exit("write error");
                    write(STDOUT_FILENO, buff, ret);
                }
                else
                {
                    perr_exit("read error");
                }
            }
        }
    }
    close(lfd);
    return 0;
}
