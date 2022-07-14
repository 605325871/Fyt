#include "TcpSocket.hh"
#include <fstream>
#include <iostream>
#include <sys/signal.h>

int main()
{
    signal(SIGPIPE, SIG_IGN);
    TcpSocket tcp;

    tcp.connectToHost("127.0.0.1", 9527);
    std::ifstream ifs("output.txt", std::ios::in);
    if (!ifs.is_open())
        std::cerr << "eror open" << std::endl;
    std::string sst;
    std::string s;  
    while (1)
    {
        while (ifs >> s)
        {
            tcp.sendMsg(s);
            sst = tcp.recvMsg();
            std::cout << sst.size() << std::endl;
        }
        // std::cin >>s;
        // tcp.sendMsg(s);
    }
    ifs.close();
}