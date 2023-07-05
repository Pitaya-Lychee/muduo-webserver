# include"acceptor.h"

#include<assert.h>
#include<cstring>
#include<sys/socket.h>
#include<bits/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include<functional>

#include"address.h"
#include"channel.h"

using namespace pitaya;
namespace{
    const int kMaxListenNum=5;
}

Acceptor::Acceptor(EventLoop* loop,const Address& address):
    loop_(loop),
    listenfd_(socket(PF_INET,SOCK_STREAM,0)),
    channel_(new Channel(loop,listenfd_)){
        BindListenFd(address);
        channel_->SetReadCallback(std::bind(&Acceptor::NewConnection,this));
        //当channel可读立即执行Acceptor::NewConnection
}

void Acceptor::BindListenFd(const Address& addr){
    struct sockaddr_in address;
    memset(&address,0,sizeof(address));
    address.sin_family=AF_INET;
    address.sin_port=htons(addr.port());
    inet_pton(AF_INET,addr.ip(),&address.sin_addr);
    int ret=bind(listenfd_, (struct sockaddr*)(&address),sizeof(address));
    assert(ret!=-1);
}

void Acceptor::Listen(){
    int ret=listen(listenfd_,kMaxListenNum);
    assert(ret!=-1);
    channel_->EnableReading();
}

void Acceptor::NewConnection(){
    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int connfd=accept(listenfd_,(struct sockaddr*)(&client),&client_addrlength);
    new_connection_callback_(connfd);
}

