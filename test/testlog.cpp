#include"../base/log.h"
#include<vector>
#include<thread>
#include <memory>
void testSubmit(const int &num){
    
    for(int i=0;i<1000;i++){
        auto& log=doubleBufferLog::instance();
        log.submit("["+std::to_string(num)+":"+std::to_string(i)+"]"+"***********************************************\n");
    }
}
int main(){
    std::vector<std::unique_ptr<std::thread> >a;
    for(int i=0;i<1000;i++){
        a.emplace_back(new std::thread(testSubmit,i));
    }
    for(int i=0;i<1000;i++){
        a[i]->join();
    }
    return 0;
}

// class Poll{
//     public:
//         Poll(){}
//         virtual bool channel(int op,int fd ,int listen)=0;
//         virtual int wait()=0;
// };