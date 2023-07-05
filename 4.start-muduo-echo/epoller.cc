#include"epoller.h"
#include<string.h>
#include<sys/epoll.h>
#include<fcntl.h>


#include"channel.h"

using namespace pitaya;

Epoller::Epoller()
    :epollfd_(epoll_create(kMaxEvents)),
     events_(kMaxEvents){

     }

void Epoller::Poll(Channels&  channels){
    printf("Epoller Poll start\n");
    int eventnums=EpollWait();
    FillActiveChannels(eventnums,channels);
    printf("Epoller Poll end\n");
}

void  Epoller::FillActiveChannels(int eventnums,Channels& channels){
    for(int i=0;i<eventnums;++i){
        Channel* ptr = static_cast<Channel*>(events_[i].data.ptr);
        ptr->SetReceivedEvents(events_[i].events);
        channels.emplace_back(ptr);
    }
}

int Epoller::SetNonBlocking(int  fd){
    int old_state=fcntl(fd,F_GETFL);
    int new_state=old_state| O_NONBLOCK;
    fcntl(fd,F_SETFL,new_state);
    return new_state;
}

void Epoller::Update(Channel* channel){
    int op=0,events=channel->events();
    if(events& EPOLLIN){
        op=EPOLL_CTL_ADD;
        SetNonBlocking(channel->fd());
    }else if(events & EPOLLRDHUP){
        op = EPOLL_CTL_DEL;
    }else{

    }
    UpdateChannel(op,channel);
}

void Epoller::UpdateChannel(int operation,Channel* channel){
    struct epoll_event event;
    memset(&event,'\0',sizeof(struct epoll_event));
    event.events=channel->events()  | EPOLLET;
    event.data.ptr=static_cast<void*>(channel);
     //将一个Channel类型的指针，转化为void类型的指针，
     epoll_ctl(epollfd_,operation,channel->fd(),&event);
     return;
}