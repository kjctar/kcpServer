#include"kcpConnection.h"
kcpConnection::kcpConnection(std::string selfIp,int selfPort,std::string peerIp,int peerPort,
                std::function<void(std::string)> msgDeal,timerQueue &tmq):udpConnection(selfIp,selfPort,peerIp,peerPort,msgDeal),tmq(tmq){
        
    std::cout<<"开启kcp"<<std::endl;
    kcp = ikcp_create(socketFd, NULL);//暂定
    ikcp_nodelay(kcp, 1, 10, 2, 1);
    ikcp_wndsize(kcp, 128, 128);
    kcp->user=&socketFd;
    kcp->output = kcpConnection::kcp_output;
    
}

void kcpConnection::getMsg(){
    int  ret;
    char *recvBuffer=new char[MAXBUF];

    ret=read(socketFd,recvBuffer,MAXBUF);
    ikcp_input(kcp,recvBuffer,ret);
    ret=ikcp_recv(kcp,recvBuffer,MAXBUF);
    *(recvBuffer+ret)='\0';
    cb(recvBuffer);
    return;
}
    
void kcpConnection::sendMsg(std::string msg){
    std::cout<<"发送："<<std::endl;
    ikcp_send(kcp,msg.data(),msg.size());
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