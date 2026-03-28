#include"common.hpp"
int main(){
    //创建一个管道文件
    fifoname pipeline(PATH,Name);
    //开始文件操作
    fifoopener severe(PATH,Name);
    severe.openforwrite();
    severe.Write();
    severe.Close();
}