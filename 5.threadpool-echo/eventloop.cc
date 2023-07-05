#include "eventloop.h"

#include<stdint.h>
#include<sys/eventfd.h>
#include<pthread.h>

#include"mutex.h"
#include"channel.h"

using namespace pitaya;

EventLoop::EventLoop():
    tid_(::pthread_self()),
    epoller_(new Epoller()),
    wakeup_fd_(::eventfd(0,EFD_NONBLOCK)),
    wakeup_channel_(new Channel(this,wakeup_fd_)){
        wakeup_channel_->SetReadCallback(std::bind(&EventLoop::HandleRead,this));
        //当channel可读，立即执行EventLoop::HandleRead;
        wakeup_channel_->EnableReading();//将事件标记为EPOLLIN
    }

EventLoop::~EventLoop(){}

void EventLoop::Loop(){
    while(true){
        epoller_->Poll(active_channels_);
        for(const auto& channel:active_channels_){
            channel->HandleEvent();  //回调并执行channel中的read_callback_或者write_callback_
        }
        active_channels_.clear();
        DoToDoList(); 
        //执行其他线程加在该线程中的任务(其他线程会通过使wakeup_fd_中写数据，从而唤醒该线程)
    }
}

void EventLoop::HandleRead(){
    uint64_t read_one_byte=1;
    ::read(wakeup_fd_,&read_one_byte,sizeof(read_one_byte));
    return;
}

void EventLoop::RunOneFunc(const BasicFunc& func){
    if(IsInThreadLoop()){//如果就是当前线程所在的任务，则直接执行
        func();
    }else {
        {   //如果不是当前线程的任务，则加入to_do_list_
            MutexLockGuard lock(mutex_); //防止多个线程同时往to_do_list_中加任务
            to_do_list_.emplace_back(func);
        }
        if(!IsInThreadLoop()){
            uint64_t write_one_byte=1;//用于唤醒该任务所在的线程
            ::write(wakeup_fd_,&write_one_byte,sizeof(write_one_byte));
        }
    }
}

void  EventLoop::DoToDoList(){
    ToDoList functors;
    {
        MutexLockGuard lock(mutex_);
        functors.swap(to_do_list_);
    }
    for(const auto&func:functors){
        func();
    }
}





