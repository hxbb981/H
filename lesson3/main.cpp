#include"common.hpp"
int main(){
    //创建一个进程池
    processpool pool;
    pool.Creat(5);
    //选择一个管道发送一个任务
    int i=3;
    while(i){
        pool.run();
        i--;
    }
    //管道读任务并执行
}
