# include"httpresponse.h"

#include<stdio.h>
#include<string>

#include "buffer.h"


using namespace pitaya;
using std::string;

const string HttpResponse::server_name_="Pitaya_muduo";



/*返回头部示例
HTTP/1.1 200 OK
Date: Mon, 10 Jul 2023 12:00:00 GMT
Server: Apache/2.4.38 (Unix)
Content-Type: text/html; charset=utf-8
Content-Length: 512
*/
void HttpResponse::AppendToBuffer(Buffer* buffer){
    char buf[32]={0};
    snprintf(buf,sizeof(buf),"HTTP/1.1 %d",status_code_);
    buffer->Append(buf); //写入"HTTP/1.1 200OK"
    buffer->Append(status_message_);
    buffer->Append(CRLF);

    if(close_connection_){
        buffer->Append("Connection: close\r\n");
    }else{
        snprintf(buf,sizeof(buf),"Content-Length: %zd\r\n",body_.size());
        buffer->Append(buf);
        buffer->Append("Connection: Keep-Alive\r\n");
    }

    buffer->Append("Content-Type: ");
    buffer->Append(type_);
    buffer->Append(CRLF);

    buffer->Append("Server: ");
    buffer->Append(server_name_);
    buffer->Append(CRLF);
    buffer->Append(CRLF);

    buffer->Append(body_);//正文
    return;
}