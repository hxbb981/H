#include<iostream>
#include<functional>
using func_t=std::function<void()>;
void downtask(){
    std::cout<<"我是一个下载的任务"<<std::endl;
    return;
}
