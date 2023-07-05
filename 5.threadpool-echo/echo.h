# ifndef _ECHO_H_
# define _ECHO_H_

#include<stdio.h>
#include<string>
#include"tcpserver.h"
#include"tcpconnection.h"

static const int thread_nums=8;

namespace pitaya{

class Address;
class EventLoop;
class EchoServer{
public:
    EchoServer(pitaya::EventLoop* loop,const pitaya::Address& listen_addr);
    ~EchoServer(){}

    void Start(){
        server_.Start();
    }

    void ConnectionCallback(pitaya::TcpConnection* connection_ptr){
        printf("echo_server has a new connection \n");
    }

    void MessageCallback(pitaya::TcpConnection* connection_ptr){
        std::string message(connection_ptr->Get());
        printf("echo_server get message \n");
        connection_ptr->Send(message);
    }

private:
    pitaya::EventLoop* loop_;
    pitaya::TcpServer server_;
};

}

#endif