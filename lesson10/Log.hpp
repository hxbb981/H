#ifndef __LOG_HPP__
#define __LOG_HPP__


#include <iostream>
#include <cstdio>
#include <string>
#include <filesystem> //C++17
#include <sstream>
#include <fstream>
#include <memory>
#include <ctime>
#include<stdio.h>
#include <unistd.h>
#include "Mutex.hpp"

namespace LogMudel{
    using namespace MutexMoudel;
    const std::string grep="\r\n";
    // 2策略模式，设置两种模式，一种打印到显示屏，一种给我打印到文件中
    class LogStrategy{
    public:
        virtual void Strategy(const std::string &message)=0;//设置纯虚函数 
    };
    class screenstrategy:public LogStrategy{
    public:
        screenstrategy(){

        }
        void Strategy(const std::string&message)override{
            //override声明函数是重写的父类的函数
            LockMutex Lock(mutex);//当声名时直接加锁，因为显示器是临界资源
            std::cout<<message<<grep<<std::endl;
        }
    private:
        Mutex mutex;
    };
    const std::string pathfault="./Log";
    const std::string filefault="my.log";//非静态不能在类内进行初始化
    class filestrategy:public LogStrategy{
    public:
        filestrategy(const std::string&path=pathfault,const std::string&file=filefault)
        :_path(path)
        ,_file(file){
            LockMutex Lock(_mutex);
            if(std::filesystem::exists(_path)){
                //这个函数是用来标识路径是否存在
                return;
            }
            try{
                std::filesystem::create_directories(_path);//创造路径
            }
            catch(std::filesystem::filesystem_error&e){
                std::cout<<e.what()<<"\n"<<std::endl;
            }
        }
        void Strategy(const std::string &message){
            std::string filename=_path+(_path.back()=='/'?"":"/")+_file;
            std::ofstream out(filename,std::ios::app);//以追加的方式打开文件，out是文件指针流
            if(!out.is_open()){
                return;
            }
            out<<message<<grep;
            out.close();
        }
    private:
        std::string _path;
        std::string _file;
        Mutex _mutex;
    };
    
    //形成完整的日志


    //1:列举日志类型
    enum class Loglevel{
        DEBUG,
        INFO,
        ERROR,
        WARNING,
        FATAL
    };
    //将类型格式化
    std::string getlevel(Loglevel level){
        switch(level){
            case Loglevel::DEBUG:
                return "DEBUG";
            case Loglevel::ERROR:
                return "ERROR";
            case Loglevel::FATAL:
                return "FATAL";
            case Loglevel::INFO:
                return "INFO";
            case Loglevel::WARNING:
                return "WARNING";
            default:
                return "UNKNOWN";
        }
    }
    //获取时间戳
    const std::string GetTimeStamp()
    {
        time_t cur=time(nullptr);//获取当前系统时间，秒级
        struct tm curr;//定义储存时间的结构体
        localtime_r(&cur,&curr);//讲当前时间获取并储存在结构体中
        char timebuffer[128];
        snprintf(timebuffer,sizeof(timebuffer),"%4d-%02d-%02d-%02d-%02d-%02d",curr.tm_year,curr.tm_mon,curr.tm_mday,curr.tm_hour,
        curr.tm_min,curr.tm_sec
        );
     return timebuffer;
    }

    class Logger{
    public:
        Logger(){
            EnablescreenStrategy();
        }
        void EnablescreenStrategy(){
            _flash_strategy=std::make_unique<screenstrategy>();//将智能指针指向第一个子类
        }
        void Enablefilestrategy(){
            _flash_strategy=std::make_unique<filestrategy>();//指向第二个子类
        }
        //形成一条日志信息
        class Loggermessage{
        public:
            Loggermessage(Loglevel level,const std::string filename,int line,Logger&log)
            :src_name(filename)
            ,_level(level)
            ,_linenumber(line)
            ,_curr_time(GetTimeStamp())
            ,_log(log)
            ,_pid(getpid())
            {
                //日志的左半部分
                std::stringstream ss;
                ss<<"["<<_curr_time<<"]"
                <<"["<<getlevel(_level)<<"]"
                <<"["<<src_name<<"]"
                <<"["<<_pid<<"]"
                <<"["<<_linenumber<<"]";
                ch=ss.str();
            }
            template<class T>
            Loggermessage& operator<<(const T& date){
                std::stringstream ss;
                ss<<date;//将date以字符串的形式写入ss中
                ch+=ss.str();
                return *this;
            }
            //在Loggermessage析构时来完成打印的调用
            ~Loggermessage(){
                _log._flash_strategy->Strategy(ch);
            }
        private:
            std::string _curr_time;
            std::string src_name;
            Loglevel _level;
            int _linenumber;
            pid_t _pid;
            std::string ch;
            Logger& _log;//这里只能用引用来写
        };
        //这里故意返回临时变量
        Loggermessage operator()(Loglevel level,const std::string&filename,int line){
            return Loggermessage(level,filename,line,*this);
        }
        ~Logger(){

        }
    private:
        std::unique_ptr<LogStrategy> _flash_strategy;
    };
    Logger log;//定义全局的日志
    #define LOG(level) log(level,__FILE__,__LINE__)

}
#endif