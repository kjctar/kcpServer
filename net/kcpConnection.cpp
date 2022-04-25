#include"kcpConnection.h"
kcpConnection::kcpConnection(int fd,std::shared_ptr<Epoll> ep,timerQueue &tmq):udpConnection(fd,ep),tmq(tmq){
        
    std::cout<<"开启kcp"<<std::endl;
    kcp = ikcp_create(fd, NULL);//暂定
    ikcp_nodelay(kcp, 1, 10, 2, 1);
    ikcp_wndsize(kcp, 128, 128);
    kcp->user=&fd;
    kcp->output = kcpConnection::kcp_output;
    
}
void kcpConnection::setCB(std::function<void(char*,int)>  func){
    cb=func;
}
void kcpConnection::getMsg(int socketFd){
    int  ret;
    char *recvBuffer=new char[MAXBUF];

    ret=read(socketFd,recvBuffer,MAXBUF);
    ikcp_input(kcp,recvBuffer,ret);
    ret=ikcp_recv(kcp,recvBuffer,MAXBUF);
    cb(recvBuffer,ret);
    return;
}
    
void kcpConnection::sendMsg(char *sendBuffer,int size){
    std::cout<<"发送："<<std::endl;
    ikcp_send(kcp,sendBuffer,size);
    update();
    int nextTime=ikcp_check(kcp,clock());
    if(nextTime<=clock()){
        tmq.addTask(nextTime,std::bind(&kcpConnection::update,this));
    }
    
    return;
}


void kcpConnection::update(){
    ikcp_update(kcp,clock());
}