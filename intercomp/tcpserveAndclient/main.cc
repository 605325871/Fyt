#include<iostream>
#include "Tcpepoll.hh"
#include"TcpSocket.hh"
int main()
{
    Tcpepoll tep;
    tep.InitSocket();
    tep.acceptConn();

}