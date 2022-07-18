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
    int lfd, cfd, i, socktfd;
    int client[FD_SETSIZE];
    char buff[BUFSIZ];
    struct sockaddr_in serv_add, client_addr;
    serv_add.sin_family = AF_INET;
    serv_add.sin_port = htons(serv_port);
    serv_add.sin_addr.s_addr = htonl(INADDR_ANY);

    lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
        perr_exit("socket error ");

    int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(lfd, (struct sockaddr *)&serv_add, sizeof(serv_add)) == -1)
        perr_exit("bind error ");
    if (listen(lfd, 128) == -1)
        perr_exit("listen error ");

    int maxfd = lfd;
    int maxi = -1;
    fd_set readset, temp;

    for (int i = 0; i < FD_SETSIZE; i++)
        client[i] = -1;

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
            int i;
            cfd = accept(lfd, (struct sockaddr *)&client_addr, &client_addr_len);
            if (cfd == -1)
                perr_exit("accept error ");

            for (i = 0; i < FD_SETSIZE; ++i)
            {
                if (client[i] < 0)
                {
                    client[i] = cfd;
                    break;
                }
            }
            if (i == FD_SETSIZE)
                perr_exit("too many ");

            FD_SET(cfd, &readset);

            maxfd = cfd > maxfd ? cfd : maxfd;

            if (i > maxi)
                maxi = i;
            if (0 == --cnt)
                continue;
        }
        for (i = 0; i < maxi; i++)
        {
            if ((socktfd = client[i]) < 0)
                continue;
            if (FD_ISSET(socktfd, &temp))
            {
                int ret = read(socktfd, buff, sizeof(buff));
                if (ret == 0)
                {
                    printf(" client close ----\n");
                    FD_CLR(socktfd, &readset);
                    close(socktfd);
                    client[i] = -1;//还要从数组中移除去
                }
                else if (ret > 0)
                {
                    for (int i = 0; i < ret; i++)
                        buff[i] = toupper(buff[i]);
                    ret = write(socktfd, buff, ret);
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
