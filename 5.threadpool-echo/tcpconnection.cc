#include"tcpconnection.h"
#include<sys/types.h>
#include<sys/socket.h>

#include"channel.h"

using namespace pitaya;

TcpConnection::TcpConnection(EventLoop* loop,int connfd):
    loop_(loop),
    connfd_(connfd),
    channel_(new Channel(loop_,connfd_)){
        channel_->SetReadCallback(std::bind(&TcpConnection::HandleMessage,this));
    }

void TcpConnection::HandleMessage(){
    if(Recv()>0){
        message_callback_(this);
    }
}

void TcpConnection::Send(const string& str){
    strcpy(buf_,str.c_str());
    send(connfd_,(const void *)(buf_),sizeof(buf_),0);
}

std::string TcpConnection::Get(){
    string str(buf_);
    memset(buf_,'\0',sizeof(buf_));
    return str;
}
