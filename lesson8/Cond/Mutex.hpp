#pragma once
#include<string>
#include<vector>
#include<stdio.h>
#include<pthread.h>
namespace MutexModule{
    class Mutex{
    public:
        Mutex(){
            pthread_mutex_init(&_mutex,nullptr);
        }
        void Lock(){
            int n=pthread_mutex_lock(&_mutex);
            (void)n;
        }
        void unlock(){
            int n=pthread_mutex_unlock(&_mutex);
            (void)n;
        }
        ~Mutex(){
            pthread_mutex_destroy(&_mutex);
        }
        pthread_mutex_t*Get(){
            return &_mutex;
        }
    private:
        pthread_mutex_t _mutex;
    };

    class LockMutex{
    public:
        LockMutex(Mutex&mutex)
        :_mutex(mutex)
        {
            _mutex.Lock();
        }
        ~LockMutex(){
            _mutex.unlock();
        }
    private:
        Mutex _mutex;
    };
};
