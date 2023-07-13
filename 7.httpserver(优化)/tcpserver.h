#ifndef _TCPSERVER_H_
#define _TCPSERVER_H_


#include<memory>
#include<unordered_map>
#include"callback.h"
#include"eventloop.h"
#include"acceptor.h"
#include"eventloopthreadpool.h"
#include"tcpconnection.h"
#include"noncopyable.h"

namespace pitaya{

class Address;
class TcpServer:public NonCopyAble{
public:
    TcpServer(EventLoop* loop,const Address& address);
    ~TcpServer();
    void Start(){
        threads_->StartLoop();
        loop_->RunOneFunc(std::bind(&Acceptor::Listen,acceptor_.get()));
    }

    void SetConnectionCallback(ConnectionCallback&& callback){
        connection_callback_=std::move(callback);
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

    void HandleClose(const TcpConnectionPtr& conn);
    void HandleCloseInLoop(const TcpConnectionPtr& conn);
     void HandleNewConnection(int connfd);

private:
    typedef std::unordered_map<int,TcpConnectionPtr> ConnectionMap;
    EventLoop* loop_;
    std::unique_ptr<EventLoopThreadPool> threads_;
    std::unique_ptr<Acceptor> acceptor_;

    ConnectionCallback connection_callback_;
    MessageCallback message_callback_;
    ConnectionMap connections_;

};

}

#endif