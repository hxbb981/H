#pragma once
#include<functional>
#include<iostream>
using task_t=std::function<void()>;
void Down(){
    std::cout<<"我是一个下载的任务"<<std::endl;
}