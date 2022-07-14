#include <iostream>
#include "Tcpepoll1.2.hh"
#include "TcpSocket.hh"
void promgsignal(void)
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGPIPE);
    sigprocmask(SIG_BLOCK, &set, NULL);
}
int main()
{
    promgsignal();
    Tcpepoll tep;

    
    tep.InitSocket();
    tep.acceptConn();

}