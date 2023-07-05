#include"tcpconnection.h"
# include<sys/types.h>
#include<sys/socket.h>
#include"channel.h"

using namespace pitaya;

TcpConnection::TcpConnection(EventLoop* loop,int connfd)
    :loop_(loop),connfd_(connfd),channel_(new Channel(loop_,connfd_))
{
    channel_->SetReadCallback(std::bind(&TcpConnection::HandleMessage,this));
    // 当channel_接收到数据时，就会自动调用TcpConnection::HandleMessage函数。
}

void TcpConnection::HandleMessage(){
    if(Recv()>0){
        message_callback_(this);
    }
}

void TcpConnection::Send(const string& message){
    strcpy(buf_,message.c_str());
    send(connfd_,(const void*)(buf_),sizeof(buf_),0);
}

std::string TcpConnection::Get(){
    string message(buf_);
    memset(buf_,'\0',sizeof(buf_));
    return message;
}





