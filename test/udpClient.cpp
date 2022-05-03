#include"../net/udpConnection.h"
#include<thread>
#include<random>
#include<iostream>
void input(udpConnection *conn){
    printf("进入线程\n");
    std::string msg;
	//2 循环发送数据
    while (true)
    {
		std::cout<<"input:";
		std::cin>>msg;
        conn->sendMsg(msg);

    }
}
//build: g++ udpClient.cpp ../net/udpConnection.cpp   -std=c++11 -pthread  -o udpClient
//run :  ./udpClient ip port
int main(int argc, char * argv[])
{
	//检查命令行参数是否匹配
	if(argc != 3)
	{
		printf("请传递对方的ip和端口号");
		return -1;
	}
	
	int port = atoi(argv[2]);//从命令行获取端口号
	if( port<1025 || port>65535 )//0~1024一般给系统使用，一共可以分配到65535
	{
		printf("端口号范围应为1025~65535");
		return -1;
	}
    std::default_random_engine rad;
    rad.seed(time(0));
    int myport=rad()%10000+10000;
    std::cout<<"port:"<<myport<<std::endl;
    udpConnection *conn=new udpConnection("127.0.0.1",myport,argv[1],port,[&](std::string msg){
        std::cout<<"接受到服务器消息:"<<msg<<std::endl;
    });
    conn->connectServer();
    
    std::cout<<"启动线程"<<std::endl;
    std::thread t(input,conn);
     
	while(true){
        conn->getMsg();
    }
    return 0;
}