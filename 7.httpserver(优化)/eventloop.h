# ifndef _EVENTLOOP_H_
# define _EVENTLOOP_H_

#include<stdint.h>
#include<unistd.h>
#include<sys/eventfd.h>
#include<pthread.h>
#include<vector>
#include<functional>
#include<memory>
# include"epoller.h"
# include"mutex.h"
# include"noncopyable.h"
#include"currentthread.h"


namespace pitaya{

class Epoller;
class Channel;

class EventLoop:public NonCopyAble{
public:
    typedef  std::vector<Channel*> Channels;
    typedef std::function<void ()>BasicFunc;
    typedef std::vector<BasicFunc> ToDoList;

    EventLoop();
    ~EventLoop();

    bool IsInThreadLoop(){return CurrentThread::tid()==tid_;}
    void Update(Channel* channel){epoller_->Update(channel);}
    void Remove(Channel* channel){epoller_->Remove(channel);}

    void Loop();
    void HandleRead();
    void QueueOneFunc(BasicFunc func);
    void RunOneFunc(BasicFunc func);
    void DoToDoList();

    //pthread_t DebugShowTid(){return tid_;}

private:
    pid_t tid_;
    std::unique_ptr<Epoller> epoller_;
    int wakeup_fd_;

    std::unique_ptr<Channel> wakeup_channel_;
    bool calling_functors_;
    Channels active_channels_;
    ToDoList to_do_list_;

    MutexLock mutex_;
};

}

#endif