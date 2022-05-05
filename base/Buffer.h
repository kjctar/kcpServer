#include<vector>
#include<functional>
class Buffer
{
    private:
        int _capacity;
        int _readIndex;
        int _writeIndex;
        char *cache;
        
        void expand(){
            char *newCache=new char[2*_capacity];
            memcpy(newCache,cache,_capacity);
            _capacity*=2;
            std::swap(newCache,cache);
            delete newCache;
        }
    public:
        char &operator [](int index){
            
            return cache[index+_readIndex];
        }
        char *writeAddr(){
            return cache+_writeIndex;
        }
        char *readAddr(){
            return cache+_readIndex;
        }
        int freeSize(){
            return _capacity-_writeIndex;
        }
        
        const int size(){
            return _writeIndex-_readIndex;
        }
        const int capacity(){
            return _writeIndex-_readIndex;
        }
        void moveReadIndex(int step){
            _readIndex+=step;
            if(_readIndex==_writeIndex){
                _readIndex=0;
                _writeIndex=0;
            }
        }
        void moveWriteIndex(int step){
            _writeIndex+=step;
            if(_writeIndex==_capacity){
                expand();
            }
        }

        Buffer(int capacity=1024){
            cache=new char[capacity];
            _capacity=capacity;
        }
        
        ~Buffer(){
            delete []cache;
        }
};

