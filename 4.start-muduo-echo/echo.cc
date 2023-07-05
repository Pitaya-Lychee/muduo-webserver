# include"echo.h"
#include"tcpserver.h"
EchoServer::EchoServer(pitaya::EventLoop* loop,const pitaya::Address& listen_addr)
    : loop_(loop),server_(loop,listen_addr){
        server_.SetConnectionCallback(std::bind(&EchoServer::ConnecitonCallback,this,_1));
        server_.SetMessageCallback(std::bind(&EchoServer::MessageCallback,this,_1));
    }
