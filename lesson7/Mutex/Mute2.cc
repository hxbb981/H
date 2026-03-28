#include<pthread.h>
#include<stdio.h>
//第二种，动态创建
int tecket=100;
pthread_mutex_t mutex;
void* routine(void*args){
    char*id=(char*)args;
    while(1){
        pthread_mutex_lock(&mutex);//如果没有申请到锁此时会处于阻塞状态
        if(tecket>0){
            printf("%ssucces%d",id,tecket);
            tecket--;
            pthread_mutex_unlock(&mutex);
        }
        else{
            pthread_mutex_unlock(&mutex);
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