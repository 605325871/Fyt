#include"TcpSocket.hh"
int main()
{
    TcpSocket tcp;
    tcp.connectToHost("127.0.0.1",9527);

    while(1)
    {
        
        tcp.sendMsg("hello");
        sleep(1);
    }
    
}