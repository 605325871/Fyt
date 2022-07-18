#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#define SERV_PORT 9527
class udp_client
{
private:
    int _socketfd;
    int _serv_port;
    std::string _serv_ip;
    ssize_t _ret;
    struct sockaddr_in _client_addr;
    socklen_t _client_addr_len;

public:
    udp_client(int serv_port, std::string ip)
        : _socketfd(-1), _serv_ip(ip), _serv_port(serv_port)
    {
        _client_addr_len = sizeof(_client_addr);
    }
    ~udp_client()
    {
        if (_socketfd >= 0)
            close(_socketfd);
    }
    void udpclientinit()
    {
        _socketfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_socketfd < 0)
            perr_exit("scoket error");
        // udp 不需要绑定端口，sendto会自动分配一个
    }
    void udpclientstart()
    {
        char buff[1024];
        memset(buff, '\0', sizeof(buff));
        _client_addr.sin_family = AF_INET;
        _client_addr.sin_port = ntohs(SERV_PORT);
        inet_pton(AF_INET, "127.0.0.1", &_client_addr.sin_addr);
        std::string msg;

        if (connect(_socketfd, (struct sockaddr *)&_client_addr, _client_addr_len) < 0)
            perr_exit("connect error");

        while (1)
        {
            fgets(buff, sizeof(buff), stdin);
            _ret = sendto(_socketfd, buff, strlen(buff), 0, (struct sockaddr *)&_client_addr, _client_addr_len);
            write(STDOUT_FILENO, buff, _ret);
        }
    }
private:
    void perr_exit(const char *s)
    {
        perror(s);
        exit(EXIT_FAILURE);
    }
};
int main()
{
    udp_client uc(SERV_PORT,"127.0.0.1");

    uc.udpclientinit();
    uc.udpclientstart();
    

}