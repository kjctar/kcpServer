#include"doubleBufferLog.h"
doubleBufferLog::doubleBufferLog(int chunkSize,std::string logDir,int interval):chunkSize_(chunkSize),logDir_(logDir),interval_(interval){

        logfile.open(logDir+"log.txt", std::ios::out | std::ios::app);
        _runing=true;
    }
doubleBufferLog::~doubleBufferLog(){
    logfile.close();
}



void doubleBufferLog::stopWorker(){//停止线程
    std::cout<<"停止线程"<<std::endl;
    _runing=false;
    _cond.notify_one();
    if(wk->joinable())
    wk->join();
}
void doubleBufferLog::worker(){
    std::cout<<"worker 启动"<<std::endl;
    std::chrono::milliseconds mscond(interval_);
    while(_runing){
        {
            std::unique_lock<std::mutex>  locker(lk);
            
            _cond.wait_for(locker,mscond);//每3s执行一次
            if(curBuffer.size()>0){
                _buffers.push_front(std::move(curBuffer));
                    assert(curBuffer.size()==0);// 
            } 
            _buffers.swap(_writebuffers);//直接交换缓存指针
            // std::cout<<"写入："<<std::distance(_writebuffers.begin(), _writebuffers.end())<<std::endl;
        }
            
        _writebuffers.reverse();
        for(auto &&item:_writebuffers){
            logfile<<item<<std::endl;

        }
        _writebuffers.clear();
        
    }
}
    


void doubleBufferLog::submit(std::string && tip){//用户提交日志消息
    std::unique_lock<std::mutex>  locker(lk);
    curBuffer.append(move(tip+"\n"));
    if(curBuffer.size()>=chunkSize_){//大于参考块大小就移动到缓存链表
        _buffers.push_front(move(curBuffer));
        assert(curBuffer.size()==0);// 
    }
    _cond.notify_one();
} 