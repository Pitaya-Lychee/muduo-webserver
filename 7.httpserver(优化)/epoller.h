#ifndef _EPOLLER_H_
#define _EPOLLER_H_


#include<sys/epoll.h>
#include<vector>
#include<unordered_map>
#include"noncopyable.h"
#include<map>

namespace {
    const int kDefaultEvents=16;
}

namespace pitaya{
class Channel;

class Epoller:public NonCopyAble{
public:
    typedef std::vector<epoll_event> Events;
    typedef std::vector<Channel*> Channels;

    Epoller();
    ~Epoller();
    
    void Remove(Channel* channel_);
    void Poll(Channels& channels);
    int EpollWait() {return epoll_wait(epollfd_,&*events_.begin(),static_cast<int>(events_.size()),-1);}
    void FillActiveChannels(int eventsnums,Channels& channels);
    void Update(Channel* channel);
    void UpdateChannel(int operation,Channel* channel);
    
private:
    typedef std::unordered_map<int ,Channel*> ChannelMap;
    int epollfd_;
    Events events_;
    ChannelMap channels_;
};


}

#endif