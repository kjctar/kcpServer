
#include "net/udpServer.h"
int  main(){
    std::shared_ptr<Epoll> poll(new Epoll());
    udpServer uskt(poll);
    poll->loop();

}