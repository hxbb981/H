#pragma once
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "task.hpp"

// 先描述
class channel
{
public:
    // 初始化
    channel(int wfd, pid_t pid) : _wfd(wfd),
                                  _pid(pid)
    {
    }
    // 析构，用来清除一个管道
    ~channel()
    {
    }
    void Close()
    {
        close(_wfd);
    }
    int getwfd()
    {
        return _wfd;
    }
    pid_t getpid()
    {
        return _pid;
    }
    // 向所选择的管道文件中发送退出码
    void send(int tasknode)
    {
        char *buffer[1024];
        int w = write(_wfd, buffer, sizeof(buffer));
    }
    void Wait()
    {
        int b = waitpid(_pid, nullptr, 0);
    }

private:
    // 文件描述符以及进程pid
    int _wfd;
    pid_t _pid;
};
// 再描述
class grachannel
{
public:
    grachannel() : _next(0) {}
    // 创建管道并插入到vextor中，不能在进程池中创建
    void Insert(int wfd, pid_t pid)
    {
        size_t sum = size();
        _t[sum] = channel(wfd, pid);
    }
    size_t size()
    {
        return _t.size();
    }
    channel &select()
    {
        auto &c = _t[_next];
        _next++;
        _next = _next % (_t.size());
        return c;
    }
    void closechannel()
    {
        for (auto &chann : _t)
        {
            chann.Close();
        }
    }
    void Waitchannel(){
        for(auto&g:_t){
            g.Wait();
        }
    }

private:
    std::vector<channel> _t;
    int _next;
};
class processpool
{
public:
    // 创建进程池，传递需要的数量
    void Creat(int num)
    {
        for (int a = 0; a <= _num; a++)
        {
            int arr[2] = {0};
            int n = pipe(arr); // flag为缺省值
            if (n == -1)
            {
                perror("pipe error");
            }
            int pid = fork();
            if (pid > 0)
            {
                // 父进程写
                close(arr[0]);
                _pool.Insert(arr[1], pid);
            }
            else if (pid == 0)
            {
                // 子进程来读
                close(arr[1]);
                // 子进程获取退出码,父进程不写他处于阻塞状态,不会执行后面的代码
                work(arr[0]);
                close(arr[0]);
                exit(0);
            }

            else
            {
                perror("fork error");
            }
        }
    }
    // 注册任务
    void _register()
    {
        _tm.Register(task1);
        _tm.Register(task2);
        _tm.Register(task3);
    }
    void work(int rfd)
    {
        // 拿到操作码并执行相应的动作
        while (true)
        {
            int Code = 0;
            int n = read(rfd, &Code, sizeof(Code)); // 返回值为你读到了多少个字节
            if (n > 0)
            {
                if (n != sizeof(Code))
                {
                    continue;
                }
                std::cout << "进程为：" << getpid() << "退出码为：" << rfd << std::endl;
                _tm.Execute(rfd);
            }
            else if (n == 0)
            {
                std::cout << "子进程退出" << std::endl;
                break;
            }
            else
            {
                std::cout << "读错误" << std::endl;
            }
        }
    }
    void closewaitchannel(){
        _pool.closechannel();//先关闭再回收
        _pool.Waitchannel();

    }
    void run()
    {
        // 选择一个管道
        channel a = _pool.select();
        std::cout << "你所选择的管道为:" << a.getpid() << std::endl;
        // 选择一个任务
        int _code = _tm.Code();
        // 发送任务码
        a.send(_code);
        std::cout << "发送的操作码为:" << _code << std::endl;
    }

private:
    grachannel _pool;
    int _num;
    Task _tm;
};

