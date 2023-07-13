# ifndef _CONDITION_H_
# define _CONDITION_H_

#include<pthread.h>
#include"mutex.h"
#include "noncopyable.h"

namespace pitaya{

class Condition:public NonCopyAble{
public:
    explicit Condition(MutexLock& mutex):mutex_(mutex){
        pthread_cond_init(&cond_,nullptr);
    }

    ~Condition(){
        pthread_cond_destroy(&cond_);
    }

    bool Wait(){
        int ret=0;
        ret = pthread_cond_wait(&cond_,mutex_.mutex());//在等待时解锁mutex给别的线程，唤醒后加锁
        //当条件变量被触发后，线程会解锁互斥量，并继续执行下去。
        return ret==0;
    }

    bool Signal(){
        return pthread_cond_signal(&cond_);//唤醒等待该条件变量的一个线程
    }
    /*
    pthread_cond_signal函数只会唤醒一个线程。
    如果有多个线程在等待条件变量，可以使用pthread_cond_broadcast函数来唤醒所有等待线程。
    */

   bool BroadCast(){
    return pthread_cond_broadcast(&cond_);
   }

private:
    MutexLock& mutex_;
    pthread_cond_t cond_;
};



}




# endif