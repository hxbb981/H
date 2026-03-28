#include<iostream>
#include<unistd.h>
#include<string>
#include<pthread.h>//线程库
int flag=100;
void showpid(pthread_t &pid){
    printf("0x%lx",pid);//以16进制打印pid的值
}
std::string Formatid(const pthread_t&tid){
    char id[64];
    snprintf(id,sizeof(id),"0x%lx",tid);//将pid以16进制的形式初始化id，其中id的最大空间是sizeof(id)
    return id;
}
void *routine(void*args){
    std::string name=static_cast<const char*>(args);//将args强制转化成const char*类型
    pthread_t tid=pthread_self();//可以获取调用这个函数栈帧的id值
    int cnt=5;
    while(cnt){
        std::cout<<"我是一个新线程，我的名字是"<<name<<"我的id是"<<tid<<std::endl;
        sleep(1);
        cnt--;
        flag++;
    }
    return (void*)123;
}
int main(){
    pthread_t pid;
    int n=pthread_create(&pid,nullptr,routine,(void*)"pthread_1");//在进程中创建线程的函数，成功返回0，失败返回非零（退出码）
    //第一个参数内核会将线程的id赋值给pid，第二个参数是线程的属性默认是空，第三个参数是指线程所执行的函数入口，第四个参数是指线程传给函数参数的值
    int cnt=5;
    while(cnt){
       std::cout<<"我是main进程，我的id是"<<Formatid(pthread_self())<<std::endl;//pthread_self(),可以获取当前进程的tid
       cnt--;
    }
    return 0;
}