#include"channel.h"
#include<sys/epoll.h>

using namespace pitaya;

Channel::Channel(EventLoop* loop,const int& fd):
    loop_(loop),fd_(fd),events_(0),recv_events_(0),state_(kNew){

    }
    
Channel::~Channel() {
}

void Channel::HandleEvent(){
    if(recv_events_&(EPOLLIN | EPOLLPRI | EPOLLRDHUP)){
        if(read_callback_){
            read_callback_();
        }
    }
    if(recv_events_& EPOLLOUT){
        if(write_callback_){
            write_callback_();
        }
    }
}
