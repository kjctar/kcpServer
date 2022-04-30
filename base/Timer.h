#include <sys/timerfd.h>
#include<unistd.h>
#include<iostream>
#include<queue>
#include <chrono>
/**
 * @brief 目前用于驱动update ，当服务器端发送数据时会设置一次重传的定时时间，当下次定时到了的时候如果不需要重传就关闭该定时器，直到下次再发送数据
 * 
 * @author kjctar
 * 
 * @date date description
 * 
 * @note 2022-4-23
 *  关于 timerfd 有两种计时模式 ，一种是CLOCK_REALTIME 为 1970.1.1到目前的时间，另一种是 CLOCK_MONOTONIC 为系统启动到目前的时间，
 *  网上很多资料说 CLOCK_REALTIME 是相对时间，CLOCK_MONOTONIC 是绝对时间，我认为完全是误导，他俩都是绝对时间，所以在设置初始值时必须获取对应计时方式的绝对时间
 *  正确设置方法如下：
 *          如果是  timerfd_create(CLOCK_REALTIME,0);   
 *          那么    clock_gettime(CLOCK_REALTIME, &time.it_value);
 *                  time.it_value.tv_sec+=初始间隔时间
 * 
 *          如果是  timerfd_create(CLOCK_MONOTONIC,0);   
 *          那么    clock_gettime(CLOCK_MONOTONIC, &time.it_value);
 *                  time.it_value.tv_sec+=初始间隔时间
 *  
 * 
 */
class Timer
{
    private:
        /* data */
        int timeFd;
        
    public:
        Timer();
        int setTimer(long first,long interval);
       
        int getTimerFd();
        long now(){
           return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
        }
      
};

