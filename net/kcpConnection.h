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
        kcpConnection(std::string selfIp,int selfPort,std::string peerIp,int peerPort,
                                std::function<void(std::string)> msgDeal,timerQueue &tmq);
        
        void getMsg();
      
        void sendMsg(std::string msg);
        
        static int kcp_output(const char *buf, int len, ikcpcb *kcp, void *user){
            
            int fd=*(static_cast<int*>(user));
            write(fd,buf,len);
            
            return 0;
        }
        void update();
        
};

