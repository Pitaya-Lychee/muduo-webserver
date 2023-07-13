#include "buffer.h"

#include<sys/uio.h>

using namespace pitaya;

int Buffer::ReadFd(int fd){
    char extrabuf[65536]={0};
    struct iovec iv[2];
    const int writable=writeablebytes();
    iv[0].iov_base=beginwrite();
    iv[0].iov_len=writable;
    iv[1].iov_base=extrabuf;
    iv[1].iov_len=sizeof(extrabuf);
    /*应用层缓冲区不够，开辟一段栈空间大小，使用分散读(readv)系统调用读取数据，
    然后为buffer_开辟更大的空间，存放读到栈区的那部分数据*/
    const int iovcnt=(writable < static_cast<int>(sizeof(extrabuf))?2:1);
    /*如果static_cast<int>(sizeof(extrabuf))的值小于writable的值，
        则表达式的结果为2；否则结果为1。
    */
    int readn=readv(fd,iv,iovcnt);

    if(readn<0){
        printf("Buffer::ReadFd readn < 0 SYS_ERR\n");
    }else if(readn<=writable){
        write_index_+=readn;
    }else{ //使用栈的格外空间
        write_index_=buffer_.size();
        Append(extrabuf,readn-writable);
    }
    return readn;
}