#include"udpServer.h"
udpServer::udpServer(std::string IP,int PORT,std::shared_ptr<Epoll> ep):serverIP(IP),serverPort(PORT),poll(ep){
    msgDeal=nullptr;

    struct sockaddr_in my_addr;
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = PF_INET;
    my_addr.sin_port = htons(serverPort);
    my_addr.sin_addr.s_addr = inet_addr(serverIP.c_str());
    
    int opt = 1;
    int ret = 0;
    if ((listenFd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        exit(0);
    } else {
        printf("socket OK\n");
    }

    ret = setsockopt(listenFd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    if (ret) {
        exit(1);
    }

    ret = setsockopt(listenFd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    if (ret) {
        exit(1);
    }
    if (bind(listenFd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    } else {
        printf("IP bind OK\n");
    }
    poll->registChannel(EPOLL_CTL_ADD,tmq.getTimerFd(),EPOLLIN|EPOLLET,std::bind(&timerQueue::dealOvertimeTask,&tmq));//添加定时器监听
    poll->registChannel(EPOLL_CTL_ADD,listenFd,EPOLLIN|EPOLLET,std::bind(&udpServer::accept,this));//添加accept事件
    
}

/**
 * @brief  再创建一个udp的socket 并connect对端地址和端口
 *  
 * @return int 
 */
int udpServer::accept(){
        
        char buf[16];
        struct sockaddr_in peer_Addr;
        socklen_t cli_len = sizeof(peer_Addr);
        std::cout<<"新链接"<<std::endl;
        int ret = recvfrom(listenFd, buf, 16, 0, (struct sockaddr *)&peer_Addr, &cli_len);
        if (ret < 0) {
            return -1;
        }
        
        //创建新连接，并设置消息的回调处理
        std::unique_ptr<udpConnection> newConnection(new udpConnection(serverIP,serverPort,
                                                           inet_ntoa(peer_Addr.sin_addr),ntohs(peer_Addr.sin_port),
                                                           [&](std::string msg){
                                                                if(msgDeal!=nullptr){
                                                                    msgDeal(msg);
                                                                }
                                                            }));
        //注册新连接的读事件                                             
        poll->registChannel(EPOLL_CTL_ADD,newConnection->getFd(),EPOLLIN|EPOLLET,std::bind(&udpConnection::getMsg,newConnection.get()));
        
        connList.push_back(move(newConnection));
        return 0;
        
}

void udpServer::broadcast(std::string msg){
    for(auto &&user:connList){

        user->sendMsg(msg);
        
    }
}   
