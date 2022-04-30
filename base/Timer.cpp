#include"Timer.h"
Timer::Timer(){
    timeFd=timerfd_create(CLOCK_MONOTONIC,0);
}
int Timer::setTimer(long first,long interval){//单位ms

    struct itimerspec time;

    clock_gettime(CLOCK_MONOTONIC, &time.it_value);
    time.it_value.tv_sec+=first/1000;
    time.it_value.tv_nsec+=first%1000*1e6; 

    time.it_interval.tv_sec=interval/1000;
    time.it_interval.tv_nsec=interval%1000*1e6; 
    timerfd_settime(timeFd,TFD_TIMER_ABSTIME, &time, nullptr);
    return 0;
    
}


int Timer::getTimerFd(){
    return timeFd;
}
