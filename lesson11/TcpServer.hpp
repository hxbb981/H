#include"Common.hpp"
using namespace LogMudel;
using namespace InetAddrMoudel;
const int defaulte=-1;
class TcpServer{
public:
    TcpServer(int port)
        :_port(port)
        ,_socklisfd(defaulte)
        ,_isruning(false)
    {
        
    }

    void server(int sockfd,InetAddr addr){
        char buffer[1024];
        while(true){
            //先读
            //m>0 读取成功
            //m<0 读取失败
            //m=0 断开连接 读到文件末尾
            int m=read(sockfd,&buffer,sizeof(buffer));
            if(m>0){
                LOG(Loglevel::DEBUG)<<"read success";
                std::cout<<buffer<<std::endl;

                //写回数据
                write(sockfd,&buffer,sizeof(buffer));
            }
            else if(m==0){
                LOG(Loglevel::DEBUG)<<"disconnect";
                close(sockfd);
                break;
            }
            else{
                LOG(Loglevel::DEBUG)<<"read fail";
                close(sockfd);
                break;
            }
        }
    }


    void Init(){
        //创建套接字
        signal(SIGCHLD, SIG_IGN);//第一个参数是子进程死了会给父进程这个信号，SIG_IGN是忽略函数，告诉操作系统内核去处理进程，推荐做法
        _socklisfd=socket(AF_INET,SOCK_STREAM,0);
        if(_socklisfd<0){
            LOG(Loglevel::FATAL)<<"create fail";
            exit(SOCK_ERROR);
        }
        LOG(Loglevel::DEBUG)<<"create success";
        //绑定
        InetAddr addr(_port);
        int m=bind(_socklisfd,(const sockaddr*)&addr.Getaddr(),sizeof(addr.Getaddr()));
        if(m<0){
            LOG(Loglevel::DEBUG)<<"bind fail";
            exit(BIND_ERROR);
        }
        LOG(Loglevel::DEBUG)<<"bind success";
        //将方式设置成listen状态
        int n=listen(_socklisfd,8);
        if(n<0){
            LOG(Loglevel::DEBUG)<<"listen fail";
            exit(LISTEN_ERROR);
        }
        LOG(Loglevel::DEBUG)<<"listen success";
    }
    void Start(){
        _isruning=true;
        while(_isruning){
            //获取连接
            struct sockaddr_in clie;
            socklen_t len=sizeof(sockaddr_in);
            int sockfd=accept(_socklisfd,CONV(clie),&len);//这里的clie就是客户端的ip和端口号
            if(sockfd<0){
                LOG(Loglevel::DEBUG)<<"accept fail";
                exit(ACCEPT_ERROR);
            }
            LOG(Loglevel::DEBUG)<<"accept success";
            InetAddr addr(clie);
            //server(sockfd,addr);这里效率太低了
            //所有服务器只能排队来连接，只能一个进程对一个进程
            //这里我们采用多进程的方式
            int pid=fork();
            if(pid<0){
                LOG(Loglevel::ERROR)<<"fork error";
                exit(FORK_ERROR);
            }
            else if(pid==0){
                //子进程，是能够看到父进程的sockfd的，因为继承了父进程的虚拟地址空间
                //第二种做法
                if(fork()){
                    server(sockfd,addr);//孙子进程去执行这个函数
                }
            }
            else{
                //父进程，只能够等待子进程如果不等待则会导致成为僵尸进程
                //这样写并没有出现并发的情况，但是父进程又不得不在这等待
                pid_t rid=waitpid(pid,nullptr,0);
                (void)rid;
            }
        }
    }
private:
    int _port;
    int _socklisfd;
    bool _isruning;

};