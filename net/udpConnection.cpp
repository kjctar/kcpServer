#include"udpConnection.h"
#include<iostream>
udpConnection::udpConnection(std::string selfIp,int selfPort,std::string peerIp,int peerPort
                    ,std::function<void(std::string)> msgDeal){

        int reuse = 1;
        //初始化对端 ip和端口
        struct sockaddr_in peer_Addr;
        peer_Addr.sin_family = PF_INET;//使用IPv4协议
        peer_Addr.sin_port = htons(peerPort);//设置接收方端口号
        peer_Addr.sin_addr.s_addr = inet_addr(peerIp.c_str()); //设置接收方IP 

        struct sockaddr_in self_Addr;
        self_Addr.sin_family = PF_INET;//使用IPv4协议
        self_Addr.sin_port = htons(selfPort);//设置接收方端口号
        self_Addr.sin_addr.s_addr = inet_addr(selfIp.c_str()); //设置接收方IP 
       
        //初始化socket
        if ((socketFd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
            perror("child socket");
            exit(1);
        } 
        
        //设置 ip和端口复用
        if(setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &reuse,sizeof(reuse))){
             exit(1);
        }
        if(setsockopt(socketFd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse))){
            exit(1);
        }
        
        //绑定自己的IP地址
        if (bind(socketFd, (struct sockaddr *) &self_Addr, sizeof(struct sockaddr))){
            perror("chid bind");
            exit(1);
        } else {
        }
        //printf("对端地址:%s 端口：%d\n", inet_ntoa(peer_Addr.sin_addr),ntohs(peer_Addr.sin_port));
        //绑定对端ip和端口，
        if (connect(socketFd, (struct sockaddr *) &peer_Addr, sizeof(struct sockaddr)) == -1) {
            perror("chid connect");
            exit(1);
        } 
        cb=msgDeal;
}
 void udpConnection::getMsg(){
    int  ret;
    char *recvBuffer=new char[MAXBUF];
    ret=read(socketFd,recvBuffer,MAXBUF);
    *(recvBuffer+ret)='\0';
    cb(recvBuffer);
    return;
}
    
 void udpConnection::sendMsg(std::string msg){
  
    write(socketFd,msg.data(),msg.size());
    return;
}