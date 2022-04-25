#include<string>
#include<thread>
#include<condition_variable>
#include<mutex>
#include<string>
#include<fstream>
#include<forward_list> 
#include<cassert>
#include<thread>
#include<algorithm>
#include<iostream>
/**
 * @brief  doubleBufferLog
 * @author kjctar
 * @date 2022-4-19
 * 
 */
class doubleBufferLog
{
    private:
        doubleBufferLog(int chunkSize=1024,std::string logDir="",int interval=3000);
        ~doubleBufferLog();
       
        int chunkSize_;//参考日志块大小
        std::string logDir_;//日志目录
        std::string curBuffer;//当前日志缓存块
        std::forward_list<std::string> _buffers;//缓存列表
        std::forward_list<std::string> _writebuffers;//要写入的缓存列表
        std::fstream logfile;//日志文件名
        std::mutex lk;
        std::condition_variable _cond;
        int interval_; //worker线程 处理日志的时间间隔 默认3000ms
        std::thread *wk; //指向线程对象的指针
        bool _runing;//worker运行标志

        void stopWorker();
        void worker();
        class CGarbo{
            public:
                doubleBufferLog *_log;
                CGarbo(doubleBufferLog *log):_log(log){}
                ~CGarbo()
                {
                    std::cout<<"等待子线程结束"<<std::endl;
                    _log->stopWorker();
                }
        };

    public:
        static doubleBufferLog &instance(){
            static doubleBufferLog log(1024,"");
            static std::thread t(&doubleBufferLog::worker,&log);
            log.wk=&t;
            static CGarbo cg(&log);//cg会先与t 析构，在析构函数里停止线程
            return log;
        }
       
        doubleBufferLog(const doubleBufferLog&)=delete;
        doubleBufferLog(doubleBufferLog&&)=delete;
        doubleBufferLog& operator=(const doubleBufferLog&)=delete;
        doubleBufferLog& operator=(doubleBufferLog&&)=delete;

        void submit(std::string && tip);
};
#define LOG doubleBufferLog::instance()