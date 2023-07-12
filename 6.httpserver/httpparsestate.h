#ifndef _HTTPSTATE_H_
#define _HTTPSTATE_H_

namespace pitaya{
enum HttpRequestParseState{
    kParseRequestLine,
    kParseHeaders,
    kParseBody,
    kParseGotCompleteRequest,
    kParseErrno,
};
}

#endif