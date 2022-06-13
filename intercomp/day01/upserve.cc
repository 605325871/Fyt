#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#define SERV_PORT 9527
class upserve
{
private:
    int _cfd, _lfd;
    struct sockaddr_in _serv_addr, _client_addr;
    int _serv_prot;
    socklen_t _serv_addr_len, _client_addr_len;
    char buff[1024];
    char ipbuf[16];

public:
    upserve(int port = SERV_PORT)
        : _serv_prot(port), _cfd(-1), _lfd(-1)
    {
        memset(buff, '\0', sizeof(buff));
        memset(ipbuf, '\0', sizeof(ipbuf));
        _serv_addr_len = sizeof(_serv_addr);
        _client_addr_len = sizeof(_client_addr);
        _serv_addr.sin_family = AF_INET;
        _serv_addr.sin_port = htons(_serv_prot);
        _serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    ~upserve()
    {
        if (_cfd >= 0)
            close(_cfd);
        if (_lfd >= 0)
            close(_cfd);
    }
    void servintit()
    {
        _lfd = socket(AF_INET, SOCK_STREAM, 0);
        if (_lfd < 0)
            perr_exit("socket error:");
        if (bind(_lfd, (struct sockaddr *)&_serv_addr, _client_addr_len) < 0)
            perr_exit("bind error");
        if (listen(_lfd, 128) < 0)
            perr_exit("listen error");
        _cfd = accept(_lfd, (struct sockaddr *)&_client_addr, &_client_addr_len);
        if (_cfd < 0)
            perr_exit("accept error");
    }
    void servstart()
    {
        while (true)
        {
            ssize_t len = read(_cfd, buff, sizeof(buff));
            if (len == -1)
            {
                perr_exit("read errror");
            }
            else if (len > 0)
            {
                printf("read buf = %s\n,数量为%ld", buff,len);
                // 小写 -》 大写
                for (int i = 0; i < len; ++i)
                {
                    buff[i] = toupper(buff[i]);
                }
                printf(" -- toupper: %s\n", buff);

                
                write(_cfd, buff, len + 1);
                write(STDOUT_FILENO,buff,len+1);
            }
            else if(len ==0)
            {
                printf("client disconnect...\n");
                break;
            }
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
    upserve us(9527);
    us.servintit();
    us.servstart();
}