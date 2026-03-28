#pragma once

#include "Thread.hpp"
#include "Mutex.hpp"
#include "Cond.hpp"
#include "Log.hpp"
#include <vector>
#include <queue>
namespace ThreadpoolMoudel
{
    using namespace MutexMoudel;
    using namespace CondModule;
    using namespace LogMudel;
    using namespace ThreadMoudel;
    static const int num = 5;
    template <class T>
    class Threadpool
    {
    private:
        void WeakAllthread(){
            LockMutex Lock(_mutex);
            if(_sleepnumber)
                _cond.broadcast();
        }
        void Weakonethread(){
            _cond.Signal();
        }
    public:
        Threadpool(int number = num)
            : _num(number)
            ,_isruning(false)
            ,_sleepnumber(0)
        {
            for (int i = 0; i < _num; i++)
            {
                _pool.emplace_back([this]()
                                   {
                                       Headertask(); // 双重回调
                                   });               // 在Thread类中其他的成员变量都是自己预先初始化的，只需要传递无返回值，无参数的func函数即可
            }
        }
        //当我们需要停止线程池时
        //1：线程在等待，2：线程在等待被唤醒，3：线程在等待被唤醒
        //所以要退出时必须保证任务已经处理完了
        //
        void Headertask()
        {
            T t;
            // 每一个线程都执行的任务
            char name[128];
            pthread_getname_np(pthread_self(), name, sizeof(name));
            while (true)
            {
                {
                    LockMutex lo(_mutex);
                    while (_task.empty()&&_isruning)
                    {
                        //如果任务为空，一直循环会一直等待
                        _sleepnumber++;
                        _cond.Wait(_mutex);
                        _sleepnumber--;
                    }
                    if(_isruning&&_task.empty()){
                        break;
                    }
                    // 一定有任务
                    t=_task.front();
                    _task.pop();
                }
                //处理任务的时候在非临界区
                t();
            }
        }
        void Stop(){
            if(!_isruning)return;
            _isruning=false;
            WeakAllthread();
        }
        void Start()
        {
            if(_isruning)return;
            _isruning=true;
            for(auto thread:_pool){
                thread.Start();
            }
        }
        void Join(){
            for(auto thread:_pool)
                thread.Join();
        }
        bool Eqeue(const T& task){
            LockMutex Lock(_mutex);//这里加锁的目的是什么
            if(_isruning){
                _task.push(task);
                if(_pool.size()==_sleepnumber)Weakonethread();
                return true;
            }
            return false;
        }

    private:
        std::vector<Thread> _pool; // 储存线程类
        std::queue<T> _task;
        int _num;
        Cond _cond;
        Mutex _mutex;
        bool _isruning;
        int _sleepnumber;
    };
}
