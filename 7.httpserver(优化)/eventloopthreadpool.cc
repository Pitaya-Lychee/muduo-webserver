# include"eventloopthreadpool.h"
# include"eventloopthread.h"

using namespace pitaya;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* loop):
    base_loop_(loop),threads_(),loops_(),thread_nums_(0),next_(0){

    }


EventLoopThreadPool::~EventLoopThreadPool(){}

void EventLoopThreadPool::StartLoop(){
    for(int i=0;i<thread_nums_;i++){
        EventLoopThread* ptr=new EventLoopThread();
        threads_.emplace_back(ptr); //记录创建的各线程
        loops_.emplace_back(ptr->StartLoop()); //记录各线程创建的loop
    }
}

EventLoop* EventLoopThreadPool::NextLoop(){  //当有新的连接到来，将其送到一个EventLoop中
    EventLoop* ret = base_loop_;
    if(!loops_.empty()){
        ret=loops_[next_];
        next_=(next_+1)%thread_nums_;  //round-robin算法
    }
    return ret;
}


