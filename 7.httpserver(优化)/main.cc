#include <stdio.h>

#include <iostream>
#include <string>

#include "eventloop.h"
#include "address.h"
#include "httpserver.h"
#include "httprequest.h"
#include "httpresponse.h"

#include "httpresponsefile.h"

using namespace pitaya;

using pitaya::Method;
using pitaya::HttpStatusCode;

void HttpResponseCallback(const HttpRequest& request, HttpResponse& response) {
  if (request.method() != kGet) {
    response.SetStatusCode(k400BadRequest);
    response.SetStatusMessage("Bad Request");
    response.SetCloseConnection(true);
    return;
  }    
  
  {
    const string& path = request.path(); 
    if (path == "/") {
      response.SetStatusCode(k200OK);
      response.SetBodyType("text/html");
      response.SetBody(pitaya_website);
    } else if (path == "/hello") {
      response.SetStatusCode(k200OK);
      response.SetBodyType("text/plain");
      response.SetBody("Hello, world!\n");
    } else if (path == "/favicon.ico" || path == "/favicon") {
      response.SetStatusCode(k200OK);
      response.SetBodyType("image/png");
      response.SetBody(string(favicon, sizeof(favicon))); 
    } else {
      response.SetStatusCode(k404NotFound);
      response.SetStatusMessage("Not Found");
      response.SetCloseConnection(true);
      return;
    }
  }
}

int main( int argc , char* argv[] )
{
  if (argc <= 1)
  {
    printf( "Usage: %s portname\n", argv[0] );
    return 0;
  }

  EventLoop loop;
  Address listen_address(argv[1]);
  HttpServer server(&loop, listen_address);
  server.SetHttpResponseCallback(HttpResponseCallback);
  server.Start();
  loop.Loop();

  return 0;
}

