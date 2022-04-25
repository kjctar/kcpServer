#include"udpConnection.h"
#include"../kcp/ikcp.h"
#include"../base/timerQueue.h"
class kcpConnection:public udpConnection
{
    private:
    
        ikcpcb *kcp;
        timerQueue &tmq;
    public:
        kcpConnection()=default;
        kcpConnection(int fd,std::shared_ptr<Epoll> ep,timerQueue &tmq);
        void setCB(std::function<void(char*,int)>  func);
        void getMsg(int socketFd);
            
        void sendMsg(char *sendBuffer,int size);
        
        static int kcp_output(const char *buf, int len, ikcpcb *kcp, void *user){
            
            int fd=*(static_cast<int*>(user));
            write(fd,buf,len);
            
            return 0;
        }
        void update();
        
};

