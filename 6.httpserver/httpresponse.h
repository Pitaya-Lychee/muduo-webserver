#ifndef _HTTPRESPONSE_H_
#define _HTTPRESPONSE_H_

#include<string>
#include<utility>
#include "httprequest.h"
using std::string;

namespace pitaya{

static const string CRLF="\r\n";
enum HttpStatusCode{
    k100Continue=100,
    k200OK=200,
    k400BadRequest=400,
    k403Forbiddden=403,
    k404NotFound=404,
    k500InternalServerErrno=500
};
class Buffer;

class HttpResponse{
public:
    HttpResponse(bool close_connection):
        type_("text/plain"),
        close_connection_(close_connection){

        }
    ~HttpResponse(){}

    void SetStatusCode(HttpStatusCode status_code){status_code_=status_code;}
    void SetStatusMessage(const string& status_message){status_message_=status_message;}
    void SetCloseConnection(bool close_connection){close_connection_=close_connection;}

    void SetBodyType(const string& type){type_=type;}
    void SetBodyType(const char* type){type_=type;}
    void SetBody(const string& body){body_=body;}
    void SetBody(const char* body){body_=std::move(string(body));}
    void AppendToBuffer(Buffer* buffer);

    bool CloseConnection(){return close_connection_;}

private:
    static const string server_name_;
    HttpStatusCode status_code_;
    string status_message_;
    string headers_;
    string body_;
    string type_;
    bool close_connection_;
};

}


#endif