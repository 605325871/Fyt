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

#define EXPOERT 9527
int main(int argc, const char *argv[])
{
    // 创建用于监听的套节字
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0); //报式协议
    if (sockfd == -1)
    {
        perror("socket error");
        exit(1);
    }

    // 绑定
    struct sockaddr_in serv_addr;
    // init
    memset(&serv_addr, 0, sizeof(serv_addr));
    // bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;                // 地址族协议  ipv4
    serv_addr.sin_port = htons(EXPOERT);           // 本地端口， 需要转换为大端
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 0 是用本机的任意IP

    int ret = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (ret == -1)
    {
        perror("bind error");
        exit(1);
    }

    // 等待并接受连接请求
    struct sockaddr_in cline_addr;
    socklen_t clien_len = sizeof(cline_addr);
    socklen_t serv_len = sizeof(serv_addr);
    char ipbuf[16];
    printf("cliient ip: %s, port: %d\n",
           inet_ntop(AF_INET, &cline_addr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)),
           ntohs(cline_addr.sin_port));
    // 通信
    while (1)
    {
        // 先接收数据
        char buf[1024] = {0};

        int len = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&serv_addr, &serv_len);

        if (len == -1)
        {
            perror("read error");
            break;
        }
        else if (len > 0)
        {
            // 小写 -》 大写
            for (int i = 0; i < len; ++i)
            {
                buf[i] = toupper(buf[i]);
            }
            // 数据发送给客户端
            sendto(sockfd, buf, len, 0, (struct sockaddr *)&cline_addr, clien_len);
            write(STDOUT_FILENO, buf, len);
        }
        else if (len == 0)
        {
            printf("client disconnect ...\n");
            break;
        }
    }

    close(sockfd);

    return 0;
}