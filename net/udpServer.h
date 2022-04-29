/**
 * @brief  doubleBufferLog
 * @author kjctar
 * @date 2022-4-20
 * 
 */
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
#include"../base/timerQueue.h"
#include"kcpConnection.h"
class udpServer{
    private:
        int listenFd;
        int serverPort;
        std::string serverIP;
        std::shared_ptr<Epoll> poll;
        std::vector<std::unique_ptr<udpConnection>> connList;
        std::function<void(std::string)> msgDeal;
        std::shared_ptr<timerQueue> tmq;
        
        ConnectionType type;
    public:
      
        
        /**
         * @brief Construct a new acudp object
         * 
         */
        int listen(){
            return listenFd;
        }
        udpServer(std::string IP,int PORT,std::shared_ptr<Epoll> ep,ConnectionType type);
        
        /**
         * @brief  再创建一个udp的socket 并connect对端地址和端口
         *  
         * @return int 
         */
        int accept();
        void registMsgDeal(std::function<void(std::string)> deal){
            msgDeal=deal;
        }
    

        void broadcast(std::string msg);


};
