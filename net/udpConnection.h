#include<unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/resource.h>
#include<memory>
#include"../base/Epoll.h"
class udpConnection{
    private:
        int socketFd;
        
        std::shared_ptr<Epoll> poll;
        
       
    public:
        const int MAXBUF=1024*1024;
        //广播回调函数
        std::function<void(char*,int)> cb;
        udpConnection()=default;
        udpConnection(int fd,std::shared_ptr<Epoll> ep);
        void setCB(std::function<void(char*,int)>  func){
            cb=func;
        }
        virtual void getMsg(int socketFd);
            
        virtual void sendMsg(char *sendBuffer,int size);
        virtual ~udpConnection(){
            
        }
           

 };