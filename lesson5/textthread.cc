#include<iostream>
#include<vector>
#include<string>
#include<pthread.h>
const int num=10;
void*routine(void*args){
    //线程所执行的函数，会将第三个参数传给args
    std::string name=static_cast<const char*>(args);
    delete (char*)args;//这里为什么要强转成char*再delete?
    int cnt=5;
    while(cnt--){
        std::cout<<"新线程的名字是："<<name<<std::endl;
    }
    return nullptr;
}
int main(){
    //创建多线程
    std::vector<pthread_t> tid;//储存创建的线程的id
    for(int i=0;i<num;i++){
        pthread_t t;
        char*id=new char[64];
        snprintf(id,64,"pthread-%d",i);
        int n=pthread_create(&tid,nullptr,routine,id);
        if(n==0){
            tid.push_back(t);
        }
        else{
            continue;
        }
    }
    for(int i=0;i<num;i++){
        int n=pthread_join(tid[i],nullptr);
        if(n==0){
            std::cout<<"等待成功"<<std::endl;
        }
    }
    return 0;


}