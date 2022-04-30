#include"kcpConnection.h"
#include"../base/Timer.h"
#include<iostream>
kcpConnection::kcpConnection(std::string selfIp,int selfPort,std::string peerIp,int peerPort,
                std::function<void(std::string)> msgDeal)
                :udpConnection(selfIp,selfPort,peerIp,peerPort,msgDeal){
        
    std::cout<<"开启kcp"<<std::endl;
    //对于局域网内，使用客户端的ip加端口生成 kcp会话ID；
    kcp = ikcp_create(0x1, NULL);//暂定
    ikcp_nodelay(kcp,1, 10, 2, 1);
    ikcp_wndsize(kcp, 128, 128);
    kcp->user=&socketFd;
    kcp->output = kcpConnection::kcp_output;
    update();
    
}

void kcpConnection::getMsg(){
   
    int  ret;
    char *recvBuffer=new char[MAXBUF];
    ret=read(socketFd,recvBuffer,MAXBUF);
    *(recvBuffer+ret)='\0';
   
    std::cout<<std::endl;
    ikcp_input(kcp,recvBuffer,ret);
    ret=ikcp_recv(kcp,recvBuffer,MAXBUF);
   
    for(int i=0;i<ret;i++){
        std::cout<<recvBuffer[i];
    }
    if(cb!=nullptr){
        cb(recvBuffer);
    }
    return;
}
    
void kcpConnection::sendMsg(std::string msg){
    Timer tm;
    std::cout<<"发送："<<msg<<std::endl;
    ikcp_send(kcp,msg.data(),msg.size());
    update();
    if(addTimerTask!=nullptr){
       
        addTimerTask(ikcp_check(kcp,tm.now()),std::bind(&kcpConnection::update,this));
       
    }
    return;
}


void kcpConnection::update(){
    Timer tm;
    ikcp_update(kcp,tm.now());
}