#ifndef _EVENTLOOPTHREAD_H_
#define _EVENTLOOPTHREAD_H_


#include"thread.h"
#include"mutex.h"
#include"condition.h"

namespace pitaya{
class EventLoop;


class EventLoopThread{
public:
    EventLoopThread();
    ~EventLoopThread();

    void StartFunc();
    EventLoop* StartLoop();

private:
    EventLoop* loop_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
};

}

#endif