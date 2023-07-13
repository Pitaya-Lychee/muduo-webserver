# include"httpserver.h"
#include<functional>
using namespace pitaya;
using pitaya::Version;

HttpServer::HttpServer(pitaya::EventLoop* loop,const Address& address):
    loop_(loop),server_(loop,address){
    
    server_.SetConnectionCallback(std::bind(&HttpServer::ConnectionCallback,this,_1));
    server_.SetMessageCallback(std::bind(&HttpServer::MessageCallback,this,_1,_2));
    server_.SetThreadNums(kThreadNums);
    SetHttpResponseCallback(std::bind(&HttpServer::HttpDefaultCallback,this,_1,_2)); //短连接，回复一次httpresponse后立马断开连接
    }

HttpServer::~HttpServer() {
}

void HttpServer::MessageCallback(const TcpConnectionPtr& connection,Buffer* buffer){
    HttpContent* content=connection->GetHttpContent();
    if(connection->IsShutdown()) return;

    if(!content->ParseContent(buffer)){
        connection->Send("HTTP/1.1 400 Bad Request\r\n\r\n");
        connection->Shutdown();
        return;
    }

    if(content->GetCompleteRequest()){
        DealWithRequest(content->request(),connection); //处理请求
        content->ResetContentState(); //重置状态
    }
}

void HttpServer::DealWithRequest(const HttpRequest& request,const TcpConnectionPtr& connection){
    string connection_state=std::move(request.GetHeader("Connection"));//从请求头中获得连接状态
    bool close =(connection_state=="Close"|| (request.version()==kHttp10 && connection_state!="Keep-Alive"));
    HttpResponse response(close);
    response_callback_(request,response);
    Buffer buffer;
    response.AppendToBuffer(&buffer);
    connection->Send(&buffer);

    if(response.CloseConnection()){
        connection->Shutdown();
    }

}