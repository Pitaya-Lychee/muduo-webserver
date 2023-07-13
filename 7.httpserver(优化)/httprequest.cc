#include "httprequest.h"

#include<utility>
#include<algorithm>

#include"httpparsestate.h"

using namespace pitaya;
using pitaya::Method;
using pitaya::HttpRequestParseState;


HttpRequest::HttpRequest(){
}

HttpRequest::~HttpRequest(){
}

bool HttpRequest::ParseRequestMethod(const char* start,const char* end){
    string method(start,end);
    bool has_method=true;
      if (method == "GET") {
            method_ = kGet;
        } else if (method == "POST") {
            method_ = kPost;
        } else if (method == "PUT") {
            method_ = kPut;
        } else if (method == "DELETE") {
            method_ = kDelete;
        } else if (method == "TRACE") {
            method_ = kTrace;
        } else if (method == "OPTIONS") {
            method_ = kOptions;
        } else if (method == "CONNECT") {
            method_ = kConnect;
        } else if (method == "PATCH") {
            method_ = kPatch;
        } else {
            has_method=false;
        }
    return has_method;
}

//解析请求行： GET /path/to/resource HTTP/1.1
bool HttpRequest::ParseRequestLine(const char*start,const char* end){
    const char* space=nullptr;
    space=std::find(start,end,' ');
    if(space==end){
            return false;
        }
    //请求的方法
    if(!ParseRequestMethod(start,space)){
        return false;
    }
    start=space+1;
    space=std::find(start,end,' ');
    
    //请求的路径
    if(space==end){
        return false;
    }
    const char* query=std::find(start,end,'?');
    if(query!=end){
        path_=std::move(string(start,query));
        query_=std::move(string(query+1,space));
    }else{
        path_=std::move(string(start,space));
    }
    start=space+1;

    //协议HTTP1.0/HTTP1.1`
    bool parsehttp =(start+8==end)&&std::equal(start,end-1,http);
    if(!parsehttp || (*(end-1)!='0' &&*(end-1)!='1') ){
        version_=kUnknown;
        return false;
    }
    if(*(end-1)=='0'){
        version_=kHttp10;
    }else{
        version_=kHttp11;
    }
    
    return true;
}

bool HttpRequest::ParseBody(const char* start,const char* end){
    return true;
}

//将请求头中的key-value存储到headers_中
/*
GET /path/to/resource HTTP/1.1
Host: www.example.com
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.82 Safari/537.36
Accept: text/html,application/xhtml+xml
Content-Type: application/json
Content-Length: 128
*/
bool HttpRequest::ParseHeaders(const char* start,const char*colon,const char*end){
    const char* valid=colon+1;
    while(*valid==' '){++valid;}
    headers_[std::move(string(start,colon))]=std::move(string(valid,end));
    return true;
}

void HttpRequest::Swap(HttpRequest& req){
    method_=req.method_;
    version_=req.version_;
    path_.swap(req.path_);
    query_.swap(req.query_);
    headers_.swap(req.headers_);
}