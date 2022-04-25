#include"udpConnection.h"
#include<iostream>
udpConnection::udpConnection(int fd,std::shared_ptr<Epoll> ep):socketFd(fd),poll(ep){
    poll->registChannel(EPOLL_CTL_ADD,socketFd,EPOLLIN|EPOLLET,std::bind(&udpConnection::getMsg,this,socketFd));//可读事件
}
 void udpConnection::getMsg(int socketFd){
    int  ret;
    char *recvBuffer=new char[MAXBUF];
    ret=read(socketFd,recvBuffer,MAXBUF);
    cb(recvBuffer,ret);
    return;
}
    
 void udpConnection::sendMsg(char *sendBuffer,int size){
   //std::cout<<"发送："<<std::endl;
    sendBuffer[size]='\0';
    std::cout<<size<<" "<<sendBuffer<<std::endl;
    write(socketFd,sendBuffer,size);
    return;
}