#include"../base/Timer.h"
#include <sys/time.h>
#include <chrono>
#include <string>
#include <iostream>
#include <iomanip>
 
void testClock(Timer &tm){
    
    long start=tm.now();
     __uint64_t t;
    read(tm.getTimerFd(),&t,sizeof(__uint64_t));
    
    std::cout<<tm.now()-start<<std::endl;

}
int main(){
    Timer tm;
    tm.setTimer(5000,2000);//第一次超时时间和第一次超时后后续的超时间隔时间，单位都是ms
    while(true)
    testClock(tm);
    
    return 0;
}