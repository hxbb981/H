#include<iostream>
#include<string>
#include<vector>
#include<pthread.h>
#include<functional>
#include<cstdio>
#include<cstring>
namespace Thread{
    static uint32_t number=1;
    class Thread{
        using func_t=std::function<void()>;
        private:
            void Enabledetch(){
                isdetch=true;
            }
            void Enableruning(){
                isruning=true;
            }
            static void* Routine(void*args){
                Thread*self=static_cast<Thread*>(args);
                self->Enableruning();
                if(self->isdetch){
                    self->detch();
                }
                pthread_setname_np(self->tid,self->_name.c_str());
                self->_fun;
                return nullptr;
            }

        public:
            Thread(func_t func)
            :tid(0)
            ,isdetch(false)
            ,isruning(false)
            ,res(nullptr)
            ,_fun(func){
                _name="pthread-"+std::to_string(number++);
            }
            void detch(){
                if(isdetch)return;
                if(isruning)pthread_detach(tid);
                Enabledetch();
                return;
            }
            bool Start(){
                if(isruning)return false;
                int n=pthread_create(&tid,nullptr,Routine,this);
                if(n!=0){
                    std::cerr<<"创建失败"<<strerror(n)<<std::endl;
                    return false;
                }
                else{
                    std::cout<<"创建成功"<<_name<<std::endl;
                    return true;
                }
            }
            bool Stop(){
                if(isruning){
                    int n=pthread_cancel(tid);
                    if(n!=0){
                        std::cerr<<"停止失败"<<strerror(n)<<std::endl;
                        return false;
                    }
                    else{
                        isruning=false;
                        std::cout<<_name<<"Stop"<<std::endl;
                        return true;
                    }
                }
                else return false;
            }
            bool Join(){
                if(isdetch){
                    std::cout<<"你的线程已经分离了，不能再分离了"<<std::endl;
                    return false;
                }
                int n=pthread_join(tid,&res);
                if(n!=0){
                    std::cerr<<"等待失败"<<strerror(n)<<std::endl;
                    return false;
                }
                else{
                    std::cout<<"等待成功"<<std::endl;
                    return true;
                }

            }
            ~Thread(){

            }
        private:
            pthread_t tid;
            std::string _name;
            bool isdetch;
            bool isruning;
            void*res;
            func_t _fun;

    };
}