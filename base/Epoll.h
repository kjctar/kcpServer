#include <sys/epoll.h>
#include<vector>
#include<assert.h>
#include<errno.h>
#include<functional>
#include<unordered_map>
#ifndef EPOLL_H_
#define EPOLL_H_
class Epoll{
    private:
        int epollFd;
        std::vector<epoll_event> channel;
        std::unordered_map<int,std::pair<uint32_t,std::function<void()> > >handlers;
        int wait();
        void deal(int size);
    public:
        Epoll();
        bool registChannel(int op,int fd ,uint32_t event,std::function<void()> handler);
        void loop();
};
#endif
