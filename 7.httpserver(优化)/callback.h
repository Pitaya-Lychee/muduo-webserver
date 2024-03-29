# ifndef _CALLBACK_H_
#define _CALLBACK_H_


#include <memory>
#include <functional>

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

namespace pitaya {
  class TcpConnection;
  typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
  
  class Buffer;
  typedef std::function<void (const TcpConnectionPtr&, Buffer*)> ConnectionCallback;
  typedef std::function<void (const TcpConnectionPtr&, Buffer*)> MessageCallback;
  typedef std::function<void ()> ReadCallback;
  typedef std::function<void ()> WriteCallback;
  typedef std::function<void (const TcpConnectionPtr&)> CloseCallback;
}
#endif
