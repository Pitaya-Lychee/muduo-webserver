#ifndef  _CALLBACK_H_
#define  _CALLBACK_H_

#include<functional>

// C++11中的placeholders库，用于创建占位符主要用于函数对象和函数模板中;
// 占位符通常用于预留参数位置，等待后期指定具体的参数值，以便更加灵活地调用函数。
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

namespace pitaya{
    class TcpConnection;
    typedef std::function<void (TcpConnection*)> ConnectionCallback;
    //接受一个TcpConnection类型的参数，无返回值
    typedef std::function<void (TcpConnection*)> MessageCallback;
    typedef std::function<void ()>ReadCallback;
    typedef std::function<void ()>WriteCallback;
}

#endif