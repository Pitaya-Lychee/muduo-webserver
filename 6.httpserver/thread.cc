#include"thread.h"

#include<pthread.h>
# include"latch.h"

using namespace pitaya;

static void* ThreadRun(void* arg){
    ThreadData* ptr=static_cast<ThreadData*>(arg);
    ptr->RunOneFunc();
    delete ptr;
    return nullptr;
}

Thread::Thread(const ThreadFunc& func):pthread_id_(-1),func_(func),latch_(1){}


Thread::~Thread(){
    ::pthread_detach(pthread_id_);
}

void Thread::StartThread(){
    ThreadData* ptr=new ThreadData(func_,&latch_);
    ::pthread_create(&pthread_id_,nullptr,ThreadRun,ptr);
    latch_.Wait();
}

