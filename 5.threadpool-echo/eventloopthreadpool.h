# ifndef _EVENTLOOPTHREADPOOL_H_
# define _EVENTLOOPTHREADPOOL_H_

#include<vector>

namespace pitaya{
class EventLoopThread;
class EventLoop;

class EventLoopThreadPool{
public:
    typedef std::vector<EventLoopThread*> Thread;
    typedef std::vector<EventLoop*> Loop;

    EventLoopThreadPool(EventLoop* loop);
    ~EventLoopThreadPool();

    void SetThreadNums(int thread_nums){
        thread_nums_=thread_nums;
    }

    void StartLoop();
    EventLoop* NextLoop();

private:
    EventLoop* base_loop_;
    Thread threads_;
    Loop loops_;
    int thread_nums_;
    int next_;
};
}

#endif