# include"eventloopthread.h"
#include<pthread.h>
#include<functional>
#include"mutex.h"
#include"condition.h"
#include"eventloop.h"


using namespace pitaya;

EventLoopThread::EventLoopThread()
    : loop_(nullptr),
    thread_(std::bind(&EventLoopThread::StartFunc,this)),
    mutex_(),
    cond_(mutex_){
    }

EventLoopThread::~EventLoopThread(){}

EventLoop* EventLoopThread::StartLoop(){
    thread_.StartThread();  // 创建线程
    EventLoop* loop=nullptr;
    {
        MutexLockGuard lock(mutex_);
        while(loop_==nullptr){
            cond_.Wait();  //等待线程创建loop
        }
        loop=loop_;
    }
    return loop;
}

void EventLoopThread::StartFunc(){
    EventLoop loop;  
    {   
        MutexLockGuard lock(mutex_);
        loop_=&loop;
        cond_.Signal();
    }
    loop_->Loop();
    {
        MutexLockGuard lock(mutex_);
        loop_=nullptr;
    }
}



