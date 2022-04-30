#include"../base/Timer.h"
#include <sys/time.h>
#include "../kcp/ikcp.h"
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
    tm.setTimer(5000,0);
    testClock(tm);
    
    return 0;
}