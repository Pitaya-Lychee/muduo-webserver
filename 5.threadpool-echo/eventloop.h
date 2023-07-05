# ifndef _EVENTLOOP_H_
# define _EVENTLOOP_H_

#include<stdint.h>
#include<unistd.h>
#include<sys/eventfd.h>
#include<pthread.h>
#include<vector>
#include<functional>

# include"epoller.h"
# include"mutex.h"


namespace pitaya{

class Epoller;
class Channel;

class EventLoop{
public:

    typedef  std::vector<Channel*> Channels;
    typedef std::function<void ()>BasicFunc;
    typedef std::vector<BasicFunc> ToDoList;

    EventLoop();
    ~EventLoop();

    bool IsInThreadLoop(){return ::pthread_self()==tid_;}
    void Update(Channel* channel){epoller_->Update(channel);}

    void Loop();
    void HandleRead();
    void RunOneFunc(const BasicFunc& func);
    void DoToDoList();

    pthread_t DebugShowTid(){return tid_;}

private:
    pthread_t tid_;
    Epoller* epoller_;
    int wakeup_fd_;

    Channel* wakeup_channel_;
    Channels active_channels_;
    ToDoList to_do_list_;

    MutexLock mutex_;
};



}






#endif