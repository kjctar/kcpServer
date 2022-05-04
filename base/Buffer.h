#include<vector>
#include<functional>
template<typename T>
class Buffer
{
    private:
        int _size;
        int _readIndex;
        int _writeIndex;
        char *cache;
        
        void expand(){
            char *newCache=new char[2*_size];
            memcpy(newCache,cache,)
        }
    public:
        char &operator [](int index){
            return cache[index];
        }
        char *writeAddr(){
            return cache+_writeIndex;
        }
        char *readAddr(){
            return cache+readAddr;
        }
        int freeSize(){
            return _size-_writeIndex;
        }
        const int &size(){
            return _size();
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
            if(_writeIndex==size){

            }
        }

        Buffer(int size){
            cache=new char[size];
            _size=size;
        }
        update()
        ~Buffer(){
            delete []cache;
        }
};

