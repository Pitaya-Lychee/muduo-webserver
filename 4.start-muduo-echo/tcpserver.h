#ifndef TCPSERVER_H_
#define TCPSERVER_H_

# include"callback.h"
#include"eventloop.h"
#include"acceptor.h"

namespace pitaya{
class Address;
class EventLoopTheadPoll;
class TcpServer{
public:
    TcpServer(EventLoop* loop,const Address& address);
    ~TcpServer();

    void  Start(){
        loop_->RunOneFunc(std::bind(&Acceptor::Listen,acceptor_));
    }

    void SetConnectionCallback(const ConnectionCallback& callback){
        connection_callback_=callback;
    }

    void SetMessageCallback(const MessageCallback& callback){
        message_callback_=callback;
    }

    void NewConnection(int connfd);


private:
    EventLoop* loop_;
    EventLoopTheadPoll* threads_;
    Acceptor* acceptor_;

    ConnectionCallback connection_callback_;
    MessageCallback message_callback_;
};
}

#endif