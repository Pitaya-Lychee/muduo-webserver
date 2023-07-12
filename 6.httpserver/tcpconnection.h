# ifndef _TCPCONNECTION_H_
# define _TCPCONNECTION_H_

#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string>
# include<memory>
#include<utility>

#include"callback.h"
#include"channel.h"
#include"buffer.h"
#include"httpcontent.h"

using std::string;

namespace pitaya{
class EventLoop;

class TcpConnection:public std::enable_shared_from_this<TcpConnection>{
public:
    enum ConnectionState{
        kConnected,
        kDisconnected
    };

    TcpConnection(EventLoop* loop,int connfd);
    ~TcpConnection();

    void SetConnectionCallback(ConnectionCallback callback){
        connection_callback_=std::move(callback);
    }

    void SetMessageCallback(MessageCallback callback){
        message_callback_=std::move(callback);
    }

    void SetCloseCallback(CloseCallback callback){
        close_callback_=std::move(callback);
    }

    void ConnectionEstablished(){
        state_=kConnected;
        channel_->EnableReading();
        connection_callback_(shared_from_this(),&input_buffer_);
    }

    HttpContent* GetHttpContent() {
    return &content_;
  }

    void Shutdown();  
    bool IsShutdown() { return shutdown_state_; }
    void ConnectionDestructor();
    void HandleClose();
    void HandleMessage();
    void HandleWrite();
    void Send(Buffer* buffer);
    void Send(const string& str);
    void Send(const char* message, int len);
    void Send(const char* message) { Send(message, strlen(message)); }

    int fd() const { return fd_; }
    EventLoop* loop() const { return loop_; }
    
    private:
        EventLoop* loop_;
        int fd_;
        ConnectionState state_;
        std::unique_ptr<Channel> channel_;
        Buffer input_buffer_;
        Buffer output_buffer_;
        HttpContent content_;
        bool shutdown_state_;

        ConnectionCallback connection_callback_;
        MessageCallback message_callback_;
        CloseCallback close_callback_;
};

typedef std::shared_ptr<TcpConnection> TcpconnectionPtr;

}

#endif