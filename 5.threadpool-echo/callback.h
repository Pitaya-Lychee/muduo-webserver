# ifndef _CALLBACK_H_
#define _CALLBACK_H_


#include<functional>
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

namespace pitaya{
    class TcpConnection;
    typedef std::function<void (TcpConnection*)> ConnectionCallback;
    typedef std::function<void (TcpConnection*)> MessageCallback;
    typedef std::function<void ()> ReadCallback;
    typedef std::function<void ()> WriteCallback;
}

# endif