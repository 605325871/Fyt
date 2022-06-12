#include <iostream>
#include <unistd.h>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#define export 9999
class upserve
{
private:
    int _port;
    int _socketfd;
    struct sockaddr_in _serv_addr;
    ssize_t _ret;

public:
    upserve(int port = export)
        : _port(port), _socketfd(-1), _ret(0)
    {
        _serv_addr.sin_family = AF_INET;
        _serv_addr.sin_port = htons(_port);
        _serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    ~upserve()
    {
        if (_socketfd >= 0)
            close(_socketfd);
    }
    void udpservintt()
    {
        if ((_socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
            perr_exit("socket error");
        if (bind(_socketfd, (struct sockaddr *)&_serv_addr, sizeof(_serv_addr)) < 0)
            perr_exit("bind error");
    }
    void upserstat()
    {
        while (1)
        {
            char buff[1024];
            struct sockaddr_in _client_addr;
            socklen_t _client_len = sizeof(_client_addr);
            _ret = recvfrom(_socketfd, buff, sizeof(buff), 0, (struct sockaddr *)&_client_addr, &_client_len);
            if (_ret > 0)
            {
                printf("read buff = %s ,数量为%d\n",buff,_ret);
                for(int i=0;i<_ret;++i)
                    buff[i]=toupper(buff[i]);
                sendto(_socketfd,buff,_ret,0,(struct sockaddr* ))
            }
        }
    }
    void perr_exit(const char *S)
    {
        std::perror(S);
        exit(-1);
    }
};
