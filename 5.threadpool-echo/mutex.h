# ifndef _MUTEX_H_
# define _MUTEX_H_


# include"pthread.h"

namespace pitaya{
    
class MutexLock{
public:
    MutexLock(){
        pthread_mutex_init(&mutex_,nullptr);
    }

    ~MutexLock(){
        pthread_mutex_destroy(&mutex_);
    }

    bool Lock(){
        return pthread_mutex_lock(&mutex_)==0;
    }

    bool Unlock(){
        return pthread_mutex_unlock(&mutex_)==0;
    }

    pthread_mutex_t* mutex(){return &mutex_;}  //返回锁

private:
    pthread_mutex_t mutex_;
};

class MutexLockGuard{
public:
    MutexLockGuard(MutexLock& mutex):mutex_(mutex){
        mutex_.Lock();
    }
    ~MutexLockGuard(){
        mutex_.Unlock();
    }

private:
    MutexLock& mutex_;
};

};


# endif