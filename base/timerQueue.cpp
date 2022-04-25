#include"timerQueue.h"
void timerQueue::addTask(long  ms,std::function<void()> func){
    long curtime=clock();
  
    
    long nextTime=curtime+ms;
  
    if(!timeQueue.empty()){
        
        if(nextTime<timeQueue.top().first){
            tm.setTimer(ms,0);
        }
    }else{
        tm.setTimer(ms,0);
    }
 
    timeQueue.push(make_pair(nextTime,func));

}
void timerQueue::dealOvertimeTask(){
    long curtime=clock();
    
    while(!timeQueue.empty()&&curtime>timeQueue.top().first){
        auto task=timeQueue.top();
        timeQueue.pop();
        task.second();//执行回调函数
    }
}
int timerQueue::getTimerFd(){
    return tm.getTimerFd();
}