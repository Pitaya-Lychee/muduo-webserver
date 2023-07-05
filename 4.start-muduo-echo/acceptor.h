# ifndef ACCEPTOR_H_
# define ACCEPTOR_H_

#include<functional>

namespace pitaya{

class EventLoop;
class Address;
class Channel;

class Acceptor{
public:
    typedef std::function<void (int)> NewConnectionCallback; 
    //接受一个int类型的参数connFd，无返回值
    Acceptor(EventLoop* loop,const Address& address);

    void BindListenFd(const Address& address);
    void Listen();
    void NewConnection();

    void SetNewConnectionCallback(const NewConnectionCallback& callback){
        new_connection_callback_=callback;
    }

private:
    EventLoop* loop_;
    int listenfd_;
    Channel* channel_;

    NewConnectionCallback new_connection_callback_;
};

}

#endif
