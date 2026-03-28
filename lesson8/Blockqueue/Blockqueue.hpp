#include<iostream>
#include<string>
#include<vector>
#include<pthread.h>
#include<queue>
const int defalute=5;
    template<class T>
    class Blockqueue{
    private:
        bool isempty(){
            return p.empty();
        }
        bool isfull(){
            return p.size>=_cap;
        }
    public:
        Blockqueue(int cap=defalute)
        :_cap(cap)
        ,csleep(0)
        ,psleep(0)
        {
            pthread_mutex_init(&Mutex);
            pthread_cond_init(&ccond);
            pthread_cond_init(&pcond);
        }
        void Equeue(const T&in){
            //生产者
            pthread_mutex_lock(&Mutex);
            while(isempty()){
                psleep++;
                pthread_cond_wait(&pcond,&Mutex);
                psleep--;
            }
            p.push(in);
            if(csleep>0){
                std::cout<<"唤醒消费者"<<std::endl;
                pthread_cond_signal(&ccond);
            }

        }
        T& Top(){
            //消费者调用
            pthread_mutex_lock(&Mutex);
            while(isfull()){
                csleep++;
                pthread_cond_wait(&ccond);
                csleep--;
            }
            T out=p.front();
            p.pop();
            //此刻临界资源少了一个需要唤醒生产者
            if(psleep>0){
                pthread_cond_signal(&pcond);
                std::cout<<"唤醒生产者"<<std::endl;
            }
            return out;
        }
        ~Blockqueue(){
            pthread_mutex_destory(&Mutex);
            pthread_cond_destory(&ccond);
            pthread_cond_destory(&pcond);
        }
    private:
        std::queue<T> p;//临界资源
        int _cap;//线程数量
        pthread_mutex_t Mutex;
        pthread_cond_t ccond;
        pthread_cond_t pcond;

        int csleep;//消费者休眠的个数
        int psleep;//生产者休眠的个数

    };
