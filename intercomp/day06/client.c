tcp sticky packet
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
    int sockfd, ret;
    int conter = 10;
    char buff[BUFSIZ];
    struct sockaddr_in clint_addr;
    socklen_t clint_len = sizeof(clint_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
        myerr("socket error");
    clint_addr.sin_family = AF_INET;
    clint_addr.sin_port = htons(SERV_PORT);

    inet_pton(AF_INET, "127.0.0.1", &clint_addr.sin_addr.s_addr);
    if (connect(sockfd, (struct sockaddr *)&clint_addr, clint_len) == -1)
        myerr("connect error");
    while (1)
    {
        fgets(buff,sizeof(buff),stdin);
        ret = sendto(sockfd,buff,strlen(buff),0,(struct sockaddr*)&clint_addr,clint_len);
        write(STDOUT_FILENO, buff, ret);
    }
    close(sockfd);
    return 0;
}