#include<stdio.h>
#include<pthread.h>
int ticket=100;
//创建锁的第一种方法，静态创建
pthread_mutex_t lock= PTHREAD_MUTEX_INITIALIZER;
    void*routine(void*args){
        char*id=(char*)args;
        while(1){
            pthread_mutex_lock(&lock);//申请锁
            if(ticket>0){
                printf("%s,抢到票%d",id,ticket);
                ticket--;
                pthread_mutex_unlock(&lock);//解锁
            }
        }
        return nullptr;
    }
    int main(){
        pthread_t t1,t2,t3,t4;
        pthread_create(&t1,nullptr,routine,(void*)"thread1");
        pthread_create(&t2,nullptr,routine,(void*)"pthread2");
        pthread_create(&t3,nullptr,routine,(void*)"pthread3");
        pthread_create(&t4,nullptr,routine,(void*)"pthread4");

        pthread_join(t1,nullptr);
        pthread_join(t2,nullptr);
        pthread_join(t3,nullptr);
        pthread_join(t4,nullptr);
        return 0;

    }