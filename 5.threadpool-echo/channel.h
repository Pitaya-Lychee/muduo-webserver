#ifndef _CHANNEL_H_
#define _CHANNEL_H_

# include<sys/epoll.h>
#include"callback.h"
#include"eventloop.h"

namespace pitaya{

class Channel{
public:
    Channel(EventLoop* loop,const int& fd);
    ~Channel();

    void HandleEvent();

    void SetReadCallback(const ReadCallback& callback){
        read_callback_=callback;
    }

    void SetWriteCallback(const WriteCallback& callback){
        write_callback_=callback;
    }

    void EnableReading(){
        events_|=EPOLLIN;
        Update();
    }

    void EnableWrite(){
        events_|=EPOLLOUT;
        Update();
    }

    void SetReceivedEvents(int events){
        recv_events_=events;
    }

    void Update(){
        loop_->Update(this);
    }

    int fd(){return fd_;}
    int events(){return events_;}
    int recv_events(){return recv_events_;}

private:
    EventLoop* loop_;
    int fd_;
    int events_;
    int recv_events_;
    ReadCallback read_callback_;
    WriteCallback write_callback_;
};

}
# endif