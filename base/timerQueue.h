#include"Timer.h"
#include<queue>
#include<functional>
class timerQueue
{
    private:
       
        
        
        Timer tm;
        struct cmp{
            bool operator()(std::pair<long ,std::function<void()> > a, std::pair<long ,std::function<void()> > b){
                return a.first > b.first;
            }
        };
        std::priority_queue<std::pair<long ,std::function<void()>> ,std::vector<std::pair<long ,std::function<void()>>>,cmp> timeQueue;//< 绝对定时，回调函数 >
    public:

        timerQueue(){
            
        }
        /**
         * @brief 添加定时任务，只执行一次 ms 后执行 func
         * 
         * @param ms 
         * @param func 
         */
        void addTask(long ms,std::function<void()> func);
        void dealOvertimeTask();
        int getTimerFd();
        ~timerQueue(){

        }
};

