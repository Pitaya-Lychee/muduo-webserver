#ifndef _BUFFER_H_
#define _BUFFER_H_

#include<vector>
#include<algorithm>
#include<string>
#include<assert.h>

using std::string;

namespace pitaya{
static const int kPrePendIndex=8;

class Buffer{
public:
    Buffer():buffer_(1024),read_index_(kPrePendIndex),write_index_(kPrePendIndex){
    }
    ~Buffer(){}

    int ReadFd(int fd);

    char* begin(){return &*buffer_.begin();};
    const char* begin() const{return &*buffer_.begin();};

    char* beginread(){return begin()+read_index_;}
    const char* beginread() const {return begin()+read_index_;}

    char* beginwrite(){return begin()+write_index_;}
    const char* beginwrite() const {return begin()+write_index_;}

    void Append(const char* message,int len){ //使用栈的格外空间
        MakeSureEnoughStorage(len);
        std::copy(message,message+len,beginwrite());
        write_index_+=len;
    }

    void Append(const string& message){     // 发送时装状态码、状态信息啥的
        Append(message.data(),message.size());
    }

    void Retrieve(int len){  //已经读了数据了，需要把两个索引变一下
        if(len+read_index_!=write_index_){ //读了，但没把缓冲区的数据读完
            read_index_=read_index_+len;
        }else{  //读完了，返回初始状态
            write_index_=kPrePendIndex;
            read_index_=write_index_;
        }
    }

    void RetrieveUntilIndex(const char* index){  //用于httpcontent读完后，改变read_index_的索引
        assert(beginwrite()>=index);
        read_index_+=index-beginread();
    }

    void RetrieveAll() { //读完所有数据后，改变索引
        write_index_ = kPrePendIndex;
        read_index_ = write_index_;
    }

    string RetrieveAsString(int len){
        assert(read_index_+len<=write_index_);
        string ret=std::move(PeekAsString(len));
        Retrieve(len);
        return ret;
    }

    string RetrieveAllAsString(){
        string ret=std::move(PeekAllAsString());
        RetrieveAll();
        return ret;
    }

    const char* Peek() const{
        return beginread();
    }

    char* Peek(){
        return beginread();
    }

    string PeekAsString(int len){
        return string(beginread(),beginread()+len);
    }

    string PeekAllAsString() {
        return string(beginread(), beginwrite()); 
    }

    int readablebytes() const {return write_index_-read_index_;}
    int writeablebytes() const {return buffer_.capacity()-write_index_;}
    int prependablebytes() const {return read_index_;}

    void MakeSureEnoughStorage(int len){
        if(writeablebytes()>=len) return;  //如果可写的数据长度大于请求写的长度，就直接用
        if(writeablebytes()+prependablebytes()>=kPrePendIndex+len){  
        /*因为read_index_也是在变的，可能读了几个字节后，往后延了。
        这里是将(beginread(),beginwrite())搬到为(begin()+8,begin()+8+beginwrite()-beginread()),并将read_index_重置为8；*/
            std::copy(beginread(),beginwrite(),begin()+kPrePendIndex);
            write_index_=8+readablebytes();
            read_index_=8;
        }else{
            buffer_.resize(buffer_.size()+len); //拓宽buffer的长度
        }
    }

private:
    std::vector<char> buffer_;
    int read_index_;
    int write_index_;
};

}



#endif