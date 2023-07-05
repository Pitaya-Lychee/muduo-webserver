#include"tcpserver.h"
#include"tcpconnection.h"
#include"acceptor.h"
#include"eventloopthreadpool.h"

using namespace pitaya;

TcpServer::TcpServer(EventLoop* loop,const Address& address):
    loop_(loop),acceptor_(new Acceptor(loop_,address)),
    threads_(new EventLoopThreadPool(loop_)){
        acceptor_->SetNewConnectionCallback(std::bind(&TcpServer::NewConnection,this,_1));
    }

void TcpServer::NewConnection(int connfd){
    EventLoop* loop=threads_->NextLoop();
    printf("TcpServer NewConnection Arrive Tid:%ld Manage\n",loop_->DebugShowTid());

    TcpConnection* ptr=new TcpConnection(loop,connfd);
    ptr->SetConnectionCallback(connection_callback_);
    ptr->SetMessageCallback(message_callback_);
    loop->RunOneFunc(std::bind(&TcpConnection::ConnectionEstablished,ptr));
}

