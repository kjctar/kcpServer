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
#include<functional>
/**
 * @brief udp是没有连接概念的，那么这里就是实现模拟tcp连接的udp
 *        有以下几点：
 *                  1、端口复用
 *                  2、消息处理回调注册
 *                  3、是否非阻塞
 * 
 */
class udpConnection{
    protected:
        int socketFd;
    public:
        const int MAXBUF=1024*1024;
        void *user;//指向用户结构 ，用来自定义连接信息
        //广播回调函数
        std::function<void(std::string)> cb;
        udpConnection()=default;
        udpConnection(std::string selfIp,int selfPort,std::string peerIp,int peerPort,
                            std::function<void(std::string)> msgDeal);
       
        int getFd(){
            return socketFd;
        }
        virtual void getMsg();
            
        virtual void sendMsg(std::string msg);
        virtual ~udpConnection(){
            close(socketFd);
        }

           

 };