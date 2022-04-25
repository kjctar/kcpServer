#include"udpServer.h"
udpServer::udpServer(std::shared_ptr<Epoll> ep):poll(ep){
    port = 8000;
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = PF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
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
        int ac_socket = -1;
        int ret = 0;
        int reuse = 1;
        char buf[16];
        struct sockaddr_in peer_addr;
        socklen_t cli_len = sizeof(peer_addr);
        std::cout<<"新链接"<<std::endl;
        ret = recvfrom(listenFd, buf, 16, 0, (struct sockaddr *)&peer_addr, &cli_len);
        if (ret > 0) {
        }
        
        if ((ac_socket = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
            perror("child socket");
            exit(1);
        } else {
            printf("parent:%d  new:%d\n", listenFd, ac_socket);
            
        }
        
        ret = setsockopt(ac_socket, SOL_SOCKET, SO_REUSEADDR, &reuse,sizeof(reuse));
        if (ret) {
            exit(1);
        }
        
        ret = setsockopt(ac_socket, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse));
        if (ret) {
            exit(1);
        }
        
        ret = bind(ac_socket, (struct sockaddr *) &my_addr, sizeof(struct sockaddr));
        if (ret){
            perror("chid bind");
            exit(1);
        } else {
        }
        
        peer_addr.sin_family = PF_INET;
        printf("对端地址:%s 端口：%d\n", inet_ntoa(peer_addr.sin_addr),ntohs(peer_addr.sin_port));
        if (connect(ac_socket, (struct sockaddr *) &peer_addr, sizeof(struct sockaddr)) == -1) {
            perror("chid connect");
            exit(1);
        } else {

        }
        
            //这里解析出 链接请求的 是否开启kcp 如果开启kcp 将相关配置 读取出来 ，包含kcp回话id，以及模式
        std::function<void(char*,int)> cb=std::bind(&udpServer::broadcast,this,std::placeholders::_1,std::placeholders::_2);

        std::unique_ptr<udpConnection> t(new udpConnection(ac_socket,poll));
        t->setCB(cb);
        connList[ac_socket]=std::move(t);

        return 0;
        
}

void udpServer::relay(int id,char *buffer,int size){
    int mod=1;
    if(mod==1){
        connList[id]->sendMsg(buffer,size);
    }
}

void udpServer::broadcast(char *buffer,int size){
    *(buffer+size)='\0';
    std::cout<<"广播："<<buffer<<std::endl;
    for(auto &&user:connList){

        user.second->sendMsg(buffer,size);
        
    }
}   