#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int  main(int argc,char* argv[]){
    if(argc<=2){
        printf("Usage:%s ip_address portname\n",argv[0]);
    }
    const char *ip=argv[1];
    int port=atoi(argv[2]);

    int listenfd=socket(PF_INET,SOCK_STREAM,0);  // 创建监听套接字，0表示TCP协议
    assert(listenfd>=1);

    // 将本地地址和端口转换为网络ip格式
    struct sockaddr_in address;
    memset(&address,0,sizeof(address));
    address.sin_family=AF_INET;
    address.sin_port=htons(port);
    inet_pton(AF_INET,ip,&address.sin_addr);

    // 绑定监听套接字到本地ip
    int ret=0;
    ret = bind(listenfd,(struct sockaddr*)(&address),sizeof(address));
    assert(ret!=-1);

    // 开始监听
    ret=listen(listenfd,5);
    assert(ret!=-1);

    //创建客户端网络ip地址格式
    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);

    // 创建accpet套接字
    int sockfd = accept(listenfd,(struct sockaddr*)(&address),&client_addrlength); 

    //创建缓冲区并接收
    char buf[1024]={0};
    int recv_size=0;
    recv_size=recv(sockfd,buf,sizeof(buf),0);

    //发送缓冲区中的数据
    int send_size=0;
    send_size=send(sockfd,buf,recv_size,0);

    close(sockfd);
    close(listenfd);
    return 0;
}