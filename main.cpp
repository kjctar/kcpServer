
#include "net/udpServer.h"

int  main(){
    std::shared_ptr<Epoll> poll(new Epoll());
    udpServer server("127.0.0.1",8000,poll);
    server.registMsgDeal([&](std::string msg){
        std::cout<<"处理消息："<<msg<<std::endl;
        server.broadcast(msg);
    });
    
    poll->loop();

}