#include"../base/timer.h"
int main(){
    Timer tm;
 
    tm.setTimer(0,5000);
    tm.setTimer(5000,5000);
    sleep(2);
    tm.setTimer(0,5000);
    while(true){
    
        __uint64_t t;
        int size=read(tm.getTimerFd(),&t,sizeof(__uint64_t));
        std::cout<<size<<std::endl;
    }
    return 0;
}