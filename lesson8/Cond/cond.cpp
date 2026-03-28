#include<iostream>
#include<vector>
#include<string>
#include<unistd.h>
#define NUM 5
int cnt=100;
pthread_mutex_t Mutex=PTHREAD_MUTEX_INITIALIZER;//定义锁
pthread_cond_t Cond=PTHREAD_COND_INITIALIZER;//定义条件变量
void*routine(void*args){
    std::string name=static_cast<char*>(args);
    while(1){
        pthread_mutex_lock(&Mutex);//申请锁,当其它线程没有锁的时候，必须在这里给我阻塞住
        pthread_cond_wait(&Cond,&Mutex);//释放所申请的锁，并让当前进程阻塞在条件变量下，直至被唤醒。
        std::cout<<name<<cnt<<std::endl;
        cnt++;
        pthread_mutex_unlock(&Mutex);
    }
}
int main(){
    std::vector<pthread_t>tid;
    for(int i=0;i<NUM;i++){
        pthread_t t;
        char*name=new char[64];
        snprintf(name,32,"pthread-%d",i);
        int n=pthread_create(&t,nullptr,routine,name);
        tid.push_back(t);
    }//创建5个线程
    while(true){
        //每个一秒唤醒一个线程
        std::cout<<"唤醒一个线程"<<std::endl;
        pthread_cond_signal(&Cond);
        sleep(1);
        //pthread_cond_broadcast(&Cond),唤醒在此条件变量下的所有进程
    }
    for(auto&i:tid){
        int n=pthread_join(i,nullptr);
        (void)n;
    }
    return 0;
}