#ifndef _TCPSERVER_H_
#define _TCPSERVER_H_

#include"callback.h"
#include"eventloop.h"
#include"acceptor.h"
#include"eventloopthreadpool.h"

namespace pitaya{

class Address;
class TcpServer{
public:
    TcpServer(EventLoop* loop,const Address& address);
    
    void Start(){
        threads_->StartLoop();
        loop_->RunOneFunc(std::bind(&Acceptor::Listen,acceptor_));
    }

    void SetConnectionCallback(const ConnectionCallback& callback){
        connection_callback_=callback;
    }

     void SetMessageCallback(const MessageCallback& callback){
        message_callback_=callback;
    }

     void SetThreadNums(int thread_nums){
        threads_->SetThreadNums(thread_nums);
     }

     void NewConnection(int connfd);

private:
    EventLoop* loop_;
    EventLoopThreadPool* threads_;
    Acceptor* acceptor_;

    ConnectionCallback connection_callback_;
    MessageCallback message_callback_;

};

}

#endif