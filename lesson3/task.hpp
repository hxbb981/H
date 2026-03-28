#include"common.hpp"
//先定义几个函数然后再将这几个函数的指针存放在vector中
typedef void (*task_t)();//定义一个函数指针类型所指向的函数为void()的
void task1(){
    std::cout<<"你执行了任务1"<<std::endl;
}
void task2(){
    std::cout<<"你执行了任务2"<<std::endl;
}
void task3(){
    std::cout<<"你执行了任务3"<<std::endl;
}
//面向对象将这些函数统一起来
class Task{
public:
Task(){
    srand(time(nullptr));
}
void Register(task_t t){
    _task.push_back(t);
}
int Code(){
    return rand()&_task.size();
}
void Execute(int code){
    *_task[code];
}
private:
std::vector<task_t> _task;
};
