#include"Epoll.h"
#include"doubleBufferLog.h"
Epoll::Epoll(){
    epollFd=epoll_create(1024);
}
int Epoll::wait(){
    int re=epoll_wait(epollFd,channel.data(),channel.size(),-1);
    if(re==-1){
        if(errno==EBADF){
            LOG.submit("kdpfd不是有效的文件描述符");
        }else if(errno==EFAULT){
            LOG.submit("具有写许可权不能访问事件指向的存储区");
        }else if(errno==EINTR){
            LOG.submit("在任何请求的事件发生或超时到期之前，信号处理程序中断了该调用；参见signal（7）");
        }else if(errno==EINVAL){
            LOG.submit("epfd不是epoll文件描述符，或者maxevents小于或等于零");
            
        }
    }
    
    return re;
}
void Epoll::deal(int size){
    for(int i=0;i<size;i++){
        if((channel[i].events|handlers[channel[i].data.fd].first)==handlers[channel[i].data.fd].first){
            handlers[channel[i].data.fd].second();
        }
    }   
}

bool Epoll::registChannel(int op,int fd ,uint32_t event,std::function<void()> handler){
        epoll_event et;
        et.data.fd=fd;
        et.events=event;
        
        //std::cout<<"op:"<<EPOLL_CTL_ADD<<"  fd:"<<fd<<" listen:"<<et.events<<std::endl;
        int re=epoll_ctl(epollFd,op,fd,&et);
        if(re<0){
            std::cout<<"添加失败"<<std::endl;
            std::cout<<errno<<std::endl;
            if(errno==ENOSPC){
                std::cout<<"当EPOLL_CTL_ADD添加时，已经超过了epoll的规格限制。规格限制在：/proc/sys/fs/epoll/max_user_watches。"<<std::endl;
            }
            return false;
        }
        handlers[fd]=make_pair(event,handler);
        channel.emplace_back(et);
        return true;
}
void Epoll::loop(){
    while(true){
        int size=wait();
        deal(size);
    }
} 