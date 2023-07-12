# include"acceptor.h"

#include<assert.h>
#include<cstring>
#include<sys/socket.h>
#include<bits/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<functional>
#include<fcntl.h>

#include"address.h"
#include"channel.h"

using namespace pitaya;


Acceptor::Acceptor(EventLoop* loop,const Address& address):
    loop_(loop),
    listenfd_(::socket(PF_INET,SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC,IPPROTO_TCP)),
    channel_(new Channel(loop,listenfd_)){
        int option_val=1;
        ::setsockopt(listenfd_,SOL_SOCKET,SO_KEEPALIVE,&option_val,static_cast<socklen_t>(sizeof option_val));
        BindListenFd(address);
        channel_->SetReadCallback(std::bind(&Acceptor::NewConnection,this));
        //当channel可读立即执行Acceptor::NewConnection
}

Acceptor::~Acceptor(){
    channel_->DisableAll();
    loop_->Remove(channel_.get());
    ::close(listenfd_);
}

void Acceptor::SetNonBlocking(int fd){
    int old_state=fcntl(fd,F_GETFL);
    int new_state=old_state | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_state);
    (void) new_state;
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
    int ret=listen(listenfd_,SOMAXCONN);
    assert(ret!=-1);
    channel_->EnableReading();
}

void Acceptor::NewConnection(){
    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int connfd=::accept4(listenfd_,(struct sockaddr*)(&client),&client_addrlength,SOCK_NONBLOCK|SOCK_CLOEXEC);
    int option_val=1;
    ::setsockopt(listenfd_,SOL_SOCKET,SO_KEEPALIVE,&option_val,static_cast<socklen_t>(sizeof option_val));
    assert(connfd>0);
    new_connection_callback_(connfd);
}

