#ifndef EPOLL_H_
#define EPOLL_H_


#include<sys/epoll.h>
#include<vector>

namespace{
    const int kMaxEvents = 8;
}

namespace pitaya{
class Channel;
class Epoller{
public:
    typedef std::vector<epoll_event> Events;
    typedef std::vector<Channel*> Channels;

    Epoller();

    void Poll(Channels& channels);
    int EpollWait(){return epoll_wait(epollfd_,&*events_.begin(),kMaxEvents,-1);}
    int SetNonBlocking(int fd);
    void FillActiveChannels(int eventnums,Channels& channels);
    void Update(Channel* channel);
    void UpdateChannel(int operation,Channel* channel);

private:
    int epollfd_;
    Events events_;
};

}
#endif