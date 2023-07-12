# ifndef _ACCEPTOR_H_
# define _ACCEPTOR_H_

#include<memory>
#include<functional>

namespace pitaya{
class EventLoop;
class Address;
class Channel;

class  Acceptor{
public:
    typedef std::function<void (int)> NewConnectionCallback;
    Acceptor(EventLoop* loop,const Address& address);
    ~Acceptor();

    void SetNonBlocking(int fd);
    void BindListenFd(const Address& address);
    void Listen();
    void NewConnection();
    
    void SetNewConnectionCallback(const NewConnectionCallback& callback){
        new_connection_callback_=std::move(callback);
    }

private:
    EventLoop* loop_;
    int listenfd_;
    std::unique_ptr<Channel> channel_;
    NewConnectionCallback new_connection_callback_;
};

}

# endif