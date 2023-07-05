# ifndef _ADDRESS_H_
# define _ADDRESS_H_

# include<string>
# include<cstring>

namespace pitaya{

class Address{
private:
    const char* ip_;
    const int port_;

public:
    Address(const char* port = "2022"):ip_("202.115.52.45"),port_(atoi(port)){}

    const char* ip() const{return ip_;}
    const int port() const{return port_;}

};

}

#endif