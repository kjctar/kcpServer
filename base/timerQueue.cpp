#include"timerQueue.h"
void timerQueue::addTask(long deadLine,std::function<void()> func){
    long curtime=tm.now();
   
    //std::cout<<"curtime="<<curtime<<"  deadLine="<<deadLine<<std::endl;
   
    
    if(!timeQueue.empty()){
        
        if(deadLine<timeQueue.top().first){
              // std::cout<<"添加定时："<<deadLine<<" "<<deadLine<<std::endl;
            tm.setTimer(deadLine-curtime,0);
        }
    }else{
           //std::cout<<"添加定时："<<deadLine<<std::endl;
        tm.setTimer(deadLine-curtime,0);
    }
 
    timeQueue.push(make_pair(deadLine,func));

}
void timerQueue::dealOvertimeTask(){
    long curtime=tm.now();
    
    while(!timeQueue.empty()&&curtime>=timeQueue.top().first){
        auto task=timeQueue.top();
        timeQueue.pop();
        task.second();//执行回调函数
        //std::cout<<"执行定时任务："<<std::endl;
    }
}
int timerQueue::getTimerFd(){
    return tm.getTimerFd();
}