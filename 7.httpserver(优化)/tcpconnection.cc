#include "tcpconnection.h"

#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "channel.h"
#include "buffer.h"

using namespace pitaya;

TcpConnection::TcpConnection(EventLoop* loop, int connfd) 
    : loop_(loop),
      fd_(connfd),
      state_(kDisconnected),
      channel_(new Channel(loop_, fd_)),
      shutdown_state_(false) {
  channel_->SetReadCallback(std::bind(&TcpConnection::HandleMessage, this));
  channel_->SetWriteCallback(std::bind(&TcpConnection::HandleWrite, this));
}

TcpConnection::~TcpConnection() {
  //printf("TcpConnection::~TcpConnection destructor\n");
  ::close(fd_);
}

void TcpConnection::ConnectionDestructor() {
  if (state_ == kConnected) {
    state_ = kDisconnected;
    channel_->DisableAll();  //断开连接要先关闭channel_中感兴趣的事件，再在eventloop中将channel所代表的fd移除
  }
  loop_->Remove(channel_.get());
}

void TcpConnection::HandleClose() {
  state_ = kDisconnected;
  channel_->DisableAll();

  TcpconnectionPtr guard(shared_from_this());
  close_callback_(guard);
}

void TcpConnection::HandleMessage() {
  int read_size = input_buffer_.ReadFd(fd_); //分散读
  if (read_size > 0) {
    message_callback_(shared_from_this(), &input_buffer_);
  } else if (read_size == 0) {
    HandleClose();
  }else{
    printf("TcpConneciton::HandleMessage Read SYS_Err\n");
  }
}

void TcpConnection::HandleWrite() {
  if(channel_->IsWriting()){
    int len = output_buffer_.readablebytes();//集中写
    int remaining = len;
    int send_size = ::write(fd_, output_buffer_.Peek(), remaining);
    if(send_size<0){
      assert(send_size>0);
      if(errno!=EWOULDBLOCK){
        printf("TcpConneciton::HandleWrite Write SYS_Err\n");
      }
      return;
    }

    remaining-=send_size;
    output_buffer_.Retrieve(send_size);
    assert(remaining<=len);
    if(!output_buffer_.readablebytes()){
      channel_->DisableWriting();
      }
    }
}

void TcpConnection::Send(const char* message, int len) {
  int remaining = len;
  int send_size = 0;
  if (!channel_->IsWriting() && output_buffer_.readablebytes() == 0) {
      send_size = ::write(fd_, message, len);
      if(send_size>=0){
        remaining -= send_size; 
      }else{
        if(errno !=EWOULDBLOCK){
          printf("TcpConneciton::Send Write SYS_Err\n");
        }
        return;
      } 
  }
  
  assert(remaining <= len);
  if (remaining > 0) {
    output_buffer_.Append(message + send_size, remaining);
    if (!channel_->IsWriting()) {
      channel_->EnableWriting(); 
    }
  }
}

void TcpConnection::Shutdown() { 
  shutdown_state_ = true;
  if (!channel_->IsWriting()) {
    int ret=::shutdown(fd_, SHUT_WR);
    if(ret<0){
      printf("TcpConneciton::Shutdown shutdown SYS_Err\n");
    }
  } 
}

void TcpConnection::Send(Buffer* buffer) {
  if(state_==kDisconnected) return;
  Send(buffer->Peek(), buffer->readablebytes()); 
  buffer->RetrieveAll();
}

void TcpConnection::Send(const string& message) {
  Send(message.data(), message.size());
}

