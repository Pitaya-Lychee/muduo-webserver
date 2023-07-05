#ifndef _PROCESSPOOL_H_
#define _PROCESSPOOL_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/epoll.h>

class process
{
public:
    int pid;
    int pipe[2];

    process():pid(-1),pipe{0,0}{}
};

template<typename T>
class processpool
{
    private:
        static const int MAX_EVENTS_NUMBER = 5;
        static const int MAX_USER_PER_PROCESS=10000;
        int idx;
        int listenfd;
        int epollfd;
        int max_processes_num;

        process* sub_processes;
        static processpool<T>* instance;

        processpool(int listenfd,int max_process_num=8);//构造函数

        ~processpool(){
            delete []sub_processes; 
        }

        public:
            static processpool<T>* create(int listenfd,int _max_processes_num=8){
                if(instance==nullptr){
                    instance=new processpool<T>(listenfd,_max_processes_num);
                }
                return instance;
            }

            void run();
            void run_parent();
            void run_child();
            void setup_up_sig();
};

template<typename T>
processpool<T>* processpool<T>::instance=nullptr;


template<typename T>
processpool<T>::processpool(int listenfd,int _max_processes_num): 
                        idx(-1),listenfd(listenfd),epollfd(0),
                        max_processes_num(_max_processes_num),sub_processes(nullptr)
{
    sub_processes=new process[max_processes_num];

    for(int i=0;i<max_processes_num;++i){
        socketpair(PF_UNIX,SOCK_STREAM,0,sub_processes[i].pipe);
        sub_processes[i].pid=fork();

        if(sub_processes[i].pid>0){ 
            //大于0时，说明子进程被创建成功，并关闭写端，因为此时只需要子进程在管道中读数据
            close(sub_processes[i].pipe[1]);  
            continue;
        }
        else{ 
            //为其他的时候，表示此时就为一个子进程，现在就在一个子进程中运行；
            //子进程需要关闭管道的读取端，因为此时只需要子进程向管道中写入数据
            close(sub_processes[i].pipe[0]);
            idx=i; //子进程保存自己的索引，为后面写入数据做准备
            break;
        }
    }
}

static int set_non_blocking(int fd){
    int old_state=fcntl(fd,F_GETFL);
    int new_state=old_state | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_state);
    return old_state;
}

static void addfd(int epollfd,int fd){
    epoll_event event;
    event.events=EPOLLIN | EPOLLET;
    event.data.fd=fd;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    set_non_blocking(fd);
}

static void removefd(int epollfd,int fd){
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,nullptr);
    close(fd);
}

template<typename T>
void processpool<T>::run()
{
    if(idx==-1){ //说明子进程没有可写的，只需要父进程写
        run_parent();
    }
    else{ //说明此时正在使用子进程，需要子进程写数据
        run_child();
    }
}

template<typename T>
void processpool<T>::setup_up_sig(){
    epollfd=epoll_create(5);
    assert(epollfd!=-1);
}

template<typename T>
void processpool<T>::run_parent(){
    epoll_event events[MAX_EVENTS_NUMBER];
    setup_up_sig();
    addfd(epollfd,listenfd);

    int pre_idx=0;
    int has_new_cli=1;
    int number=0;

    while(true){
        number=epoll_wait(epollfd,events,MAX_EVENTS_NUMBER,-1);
        for(int i=0;i<number;i++){
            int sockfd=events[i].data.fd;
            if(sockfd==listenfd){
                int pos=pre_idx;
                do{
                    pos=(pos+1)%max_processes_num;  //round_robin算法
                }while(sub_processes[pos].pid==-1) ;
                //判断该位置的进程是否还存在，不存在就继续循环，找到下一个存在的进程
                pre_idx=pos;

                //父进程发送给子进程的监听信息
                send(sub_processes[pos].pipe[0],(void*)&has_new_cli,sizeof(has_new_cli),0);
                printf("parent processes has sent msg to %d child\n",pos);
            }
        }
    }
}

template<typename T>
void processpool<T>::run_child()
{
    epoll_event events[MAX_EVENTS_NUMBER];
    setup_up_sig();
    int pipefd=sub_processes[idx].pipe[1];
    addfd(epollfd,pipefd);
    T *users=new T [MAX_USER_PER_PROCESS];

    int number=0;
    while(true){
        number=epoll_wait(epollfd,events,MAX_EVENTS_NUMBER,-1);
        for(int i=0;i<number;i++){
            int sockfd=events[i].data.fd;
            if(sockfd==pipefd && events[i].events & EPOLLIN){
                struct sockaddr_in client;
                socklen_t client_addrlenth=sizeof(client);
                int connfd=accept(listenfd,(struct sockaddr*)(&client),&client_addrlenth);
                addfd(epollfd,connfd);
                users[connfd].init(epollfd,connfd,client);
                printf("child %d is addfding\n",idx);  
                continue;
            }
            else if(events[i].events & EPOLLIN){
                printf("child %d has recv msg\n",idx);
                users[sockfd].process();
            }
        }
    }
    delete[] users;
    users=nullptr;
    close(epollfd);
    close(pipefd);
}

#endif 