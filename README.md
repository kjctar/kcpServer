# kcpServer
基于UDP+KCP+Epoll的消息转发服务器
#### 进展
- [x] 高并发udp消息转发
- [x] 双缓存日志
- [x] 定时器
- [x] kcp

#### 目录结构
├── base    
│   ├── doubleBufferLog.cpp //双缓存日志  
│   ├── doubleBufferLog.h  
│   ├── Epoll.cpp //epoll的封装  
│   ├── Epoll.h  
│   ├── Timer.cpp //timerfd定时器  
│   ├── Timer.h  
│   ├── timerQueue.cpp //定时任务队列  
│   └── timerQueue.h  
├── build.sh   
├── kcp   
│   ├── ikcp.c //kcp协议  
│   └── ikcp.h  
├── main  
├── main.cpp  
├── net  
│   ├── kcpConnection.cpp //kcp连接，继承于udpConnection  
│   ├── kcpConnection.h  
│   ├── udpConnection.cpp //udp连接  
│   ├── udpConnection.h  
│   ├── udpServer.cpp //监听端口，端口复用 ，创建新的udpConnection或者  kcpConnection  
│   └── udpServer.h  
├── README.md  
└── test  
    ├── kcpClient.cpp //测试kcp   
    ├── testlog.cpp //测试日志  
    ├── timerTest     
    ├── timerTest.cpp//测试定时  
    ├── udpClient  
    └── udpClient.cpp//测试udp收发  

#### Quick start

> 服务端   
> 编译：g++ main.cpp base/doubleBufferLog.cpp base/Epoll.cpp base/Timer.cpp base/timerQueue.cpp net/kcpConnection.cpp net/udpConnection.cpp net/udpServer.cpp kcp/ikcp.c -std=c++11 -pthread -o main   
> 运行：./main   
```c++
#include "net/udpServer.h"
int  main(){
    //实例化Epoll对象
    std::shared_ptr<Epoll> poll(new Epoll());
    //构造服务器；
    //参数：IP,Port,epoll智能指针对象,UDP/KCP,
    udpServer server("127.0.0.1",8000,poll,KCP);

    //注册消息回调函数，在这里写消息的业务处理，这里简单的实现群发
    server.registMsgDeal([&](std::string msg){
        std::cout<<"处理消息："<<msg<<std::endl;
        server.broadcast(msg);
    });
    //启动事件循环
    poll->loop();
}
```
>客户端   
>编译: g++ kcpClient.cpp net/kcpConnection.cpp   net/udpConnection.cpp  base/Timer.cpp kcp/ikcp.c -std=c++11 -pthread  -o kcpClient   
>运行:    ./kcpClient 127.0.0.1 8000   
```c++
#include"net/kcpConnection.h"
#include<thread>
#include<random>
#include<iostream>
void input(kcpConnection *conn){
    printf("进入线程\n");
    std::string msg;
	//2 循环发送数据
    while (true)
    {
		std::cout<<"input:";
		std::getline(std::cin,msg);
        conn->sendMsg(msg);
        conn->update();
    }
}
int main(int argc, char * argv[])
{
    if(argc != 3 || atoi(argv[2])<1025 || atoi(argv[2])>65535){
		return -1;
	}
    std::default_random_engine rad;
    rad.seed(time(0));
    int myport=rad()%10000+10000;
    std::cout<<"port:"<<myport<<std::endl;
    kcpConnection *conn=new kcpConnection("127.0.0.1",myport,argv[1],atoi(argv[2]),[&](std::string msg){
        std::cout<<"接受到服务器消息:"<<msg<<std::endl;
    });
    conn->connectServer();
    std::thread t(input,conn);
	while(true){
        conn->getMsg();
    }
    return 0;
}
```