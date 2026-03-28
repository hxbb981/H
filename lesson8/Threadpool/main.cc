
#include"Threadpool.hpp"
#include"Task.hpp"
int main(){
    using namespace LogMudel;
    using namespace ThreadpoolMoudel;
    //测试日志
    log.EnablescreenStrategy();

    LOG(Loglevel::DEBUG)<<"aaa";
    // Logger log;
    // log(Loglevel::DEBUG,"main.cc",10) << "hello world," << 3.14 << " " ;
    // Threadpool<task_t> p;//这里的task_t只是一个函数类型
    // p.Start();//这里创建了5个线程，并开始去执行headertask函数,function可以用类型去表明函数
    // for(int i=0;i<5;i++){
    //     p.Eqeue(Down);
    // }
    // //向线程池中添加了5个任务
    // p.Stop();
    // p.Join();
    // Threadpool<task_t>* m=new Threadpool<task_t>;//如果使用new的话可以让对象创建在堆区，延长生命周期
    return 0;
}