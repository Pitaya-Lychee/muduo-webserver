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
void HttpRequest::ParseRequestLine(const char*start,const char* end,HttpRequestParseState& state){
    //请求的方法
    {
        const char* space=std::find(start,end,' ');
        if(space==end){
            state=kParseErrno;
            return;
        }

        if(!ParseRequestMethod(start,space)){
            state=kParseErrno;
            return;
        }
        start=space+1;
    }
    
    //请求的路径
    {
        const char* space=std::find(start,end,' ');
        if(space==end){
            state=kParseErrno;
            return;
        }

        const char* query=std::find(start,end,'?');
        if(query!=end){
            path_=std::move(string(start,query));
            query_=std::move(string(query+1,space));
        }else{
            path_=std::move(string(start,space));
        }
        start=space+1;
    }

    //协议HTTP1.0/HTTP1.1`
    {
        const int httplen=sizeof(http)/sizeof(char)-1;
        const char* httpindex=std::search(start,end,http,http+httplen);
        if(httpindex==end){
            state=kParseErrno;
            return;
        }
        const char chr =*(httpindex+httplen);
        if(httpindex+httplen+1==end && (chr=='1' || chr=='0')){
            if(chr=='1'){
                version_=kHttp11;
            }else{
                version_=kHttp10;
            }
        }else{
            state=kParseErrno;
            return;
        }
    }

    state=kParseHeaders;
}

void HttpRequest::ParseBody(const char* start,const char* end,HttpRequestParseState& state){

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
void HttpRequest::ParseHeaders(const char* start,const char*end,HttpRequestParseState& state){
    if(start==end&&*start=='\r'&&*(start+1)=='\n'){
        state=kParseGotCompleteRequest;
        return;
    }

    const char* colon=std::find(start,end,':');
    if(colon==end){
        state=kParseErrno;
    }
    const char* valid=colon+1;
    while(*(valid++)!=' '){}
    headers_[std::move(string(start,colon))]=std::move(string(colon+1,valid));
    return;
}
