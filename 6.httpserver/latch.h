# ifndef _LATCH_H_
# define _LATCH_H_

# include "mutex.h"
#include "condition.h"

namespace pitaya{

class Latch{
public:
    Latch(int count):count_(count),mutex_(),cond_(mutex_){}

    void CountDown(){
        MutexLockGuard lock(mutex_);
        --count_;
        if(count_==0){
            cond_.BroadCast();
        }
        /*
        每当一个事件完成，线程可以通过调用latch.count_down()来减少计数器的值。
        当计数器的值减少到零时，再通过条件变量唤醒阻塞的线程。
        */
    }

    void Wait(){
        MutexLockGuard lock(mutex_);
        while(count_>0){
            cond_.Wait();
        }
    }

private:
    int count_;
    MutexLock mutex_;
    Condition cond_;
};


}






#endif