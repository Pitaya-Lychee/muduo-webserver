# include"tcpserver.h"
#include"acceptor.h"
#include"tcpconnection.h"


using namespace pitaya;

TcpServer::TcpServer(EventLoop* loop,const Address& address):
    loop_(loop),threads_(nullptr),acceptor_(new Acceptor(loop,address)){
        acceptor_->SetNewConnectionCallback(std::bind(&TcpServer::NewConnection,this,_1));
    }

TcpServer::~TcpServer(){
    delete acceptor_;
}

void TcpServer::NewConnection(int connfd){
    TcpConnection* ptr=new TcpConnection(loop_,connfd);
    ptr->SetConnectionCallback(connection_callback_);
    ptr->SetMessageCallback(message_callback_);
    loop_->RunOneFunc(std::bind(&TcpConnection::ConnectionEstablished,ptr));
}