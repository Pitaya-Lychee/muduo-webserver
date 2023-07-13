#ifndef _CHANNEL_H_
#define _CHANNEL_H_

# include<sys/epoll.h>
# include<utility>
#include"callback.h"
#include"eventloop.h"
#include "noncopyable.h"

namespace pitaya{

enum ChannelState{
    kNew,
    kAdded,
    kDeleted
};

class Channel : public NonCopyAble{
public:
    Channel(EventLoop* loop,const int& fd);
    ~Channel();

    void HandleEvent();

    void SetReadCallback(ReadCallback&& callback){
        read_callback_=std::move(callback);
    }

    void SetReadCallback(const ReadCallback& callback){
        read_callback_=callback;
    }

    void SetWriteCallback(WriteCallback&& callback){
        write_callback_=std::move(callback);
    }

    void SetWriteCallback(const WriteCallback& callback){
        write_callback_=callback;
    }

    void EnableReading(){
        events_|=(EPOLLIN|EPOLLPRI);
        Update();
    }

    void EnableWriting(){
        events_|=EPOLLOUT;
        Update();
    }

    void DisableAll(){
        events_=0;
        Update();
    }

    void DisableWriting(){
        events_&=~EPOLLOUT;
        Update();
    }


    void SetReceivedEvents(int events){
        recv_events_=events;
    }

    void Update(){
        loop_->Update(this);
    }

    void SetChannelState(ChannelState state){
        state_=state;
    }

    int fd(){return fd_;}
    int events(){return events_;}
    int recv_events(){return recv_events_;}

    ChannelState state() const {return state_;}

    bool IsWriting() {return events_ & EPOLLOUT;}
    bool IsReading() {return events_ & (EPOLLOUT|EPOLLPRI);}

private:
    EventLoop* loop_;
    int fd_;
    int events_;
    int recv_events_;

    ChannelState state_;
    ReadCallback read_callback_;
    WriteCallback write_callback_;
};

}
# endif