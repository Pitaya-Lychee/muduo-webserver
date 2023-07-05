#include"channel.h"
#include<sys/epoll.h>

using namespace pitaya;

Channel::Channel(EventLoop* loop,const int& fd):
    loop_(loop),fd_(fd),events_(0),recv_events_(0){

    }


void Channel::HandleEvent(){
    if(recv_events_&EPOLLIN){
        read_callback_();
    }else if(recv_events_& EPOLLOUT){
        write_callback_();
    }else{

    }
}
