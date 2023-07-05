#ifndef _THREAD_H_
# define _THREAD_H_

#include<pthread.h>
#include<functional>
#include"latch.h"

namespace pitaya{

class Thread{
public:
    typedef std::function<void ()> ThreadFunc;
    Thread(const ThreadFunc& func);
    ~Thread();

    void StartThread();

private:
    pthread_t pthread_id_;
    ThreadFunc func_;
    Latch latch_;
};


struct ThreadData{
typedef pitaya::Thread::ThreadFunc ThreadFunc;

ThreadFunc func_;
Latch* latch_;

ThreadData(ThreadFunc& func,Latch* latch):func_(func),latch_(latch){}

void RunOneFunc(){
    latch_->CountDown();
    latch_=nullptr;
    func_();
}
};

}
# endif