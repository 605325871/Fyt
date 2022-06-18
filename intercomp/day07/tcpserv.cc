#include <iostream>
#include<string>
using namespace std;
class TcpServer
{
public:
    TcpServer();
    ~TcpServer();

 
    int setListen(unsigned short port);

    int acceptConn(struct sockaddr_in *addr);

 
    int sendMsg(string msg);

    string recvMsg();
    
   
private:

    int readn(char* buf, int size);
  
    int writen(const char* msg, int size);
    
private:
    int lfd;	// 监听的套接字
    int cfd;	// 通信的套接字
};
