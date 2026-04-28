#include"Socket.hpp"
#include"Common.hpp"
#include<sys/poll.h>


using namespace SocketMoudel;
using namespace LogMudel;
const int defaultfd=-1;
const int size=64;
class Pollserver{

public:
    Pollserver(uint16_t port)
    :_isruning(false)
    ,_listenfd(std::make_unique<Tcpsocket>())
    {
        _listenfd->TcpsockInit(port);
        //初始化记录数组
        for(int i=0;i<64;i++){
            _pfd[i].fd=defaultfd;
            _pfd->events=0;
            _pfd->revents=0;
        }
        _pfd[0].fd=_listenfd->Fd();
        _pfd[0].events=POLLIN;
        _pfd[0].revents=0;
    }

    void Start(){
        _isruning=true;
        while(_isruning){
            //下面我们来认识poll的接口
            //第一个参数是我们用户需要取维护的一个数组，类型为pollfd结构体，第一个成员表示用户要os帮你监管哪一个文件描述符
            //第二个成员表示os维护什么事件，第三个是表示什么事件已经就绪
            //返回值与select一样
            //第三个参数，-1 永久阻塞 0feizuse >0 hs

            int n=poll(_pfd,size,10000);
           
            
            switch (n)
            {
                case 0:
                    LOG(Loglevel::ERROR)<<"time out";
                    break;
                case -1:
                    LOG(Loglevel::ERROR)<<"select error";
                    break;
                default:
                    //有事件就绪了，到底是有新连接到来，还是读事件就绪了
                    //我们当然知道，因为listenfd在这里一定是3
                    Dispatch();//处理事件
                    break;
            }
        }
    }

    void Dispatch(){
        //遍历
        for(int i=0;i<size;i++){
            if(_pfd[i].fd==defaultfd)
                continue;
            if(_pfd[i].revents&POLLIN){
                //表明事件就绪
                if(_pfd[i].fd==_listenfd->Fd()){
                    //建立连接
                    Acceptconnect();
                }
                else{
                    Recv(_pfd[i].fd,i);
                }
            }
        }
    }
    void Recv(int fd,int pos){
        //从缓冲区中取数据
        char buffer[1024];
        ssize_t n=recv(fd,buffer,sizeof(buffer),0);
        if(n>0){
            buffer[n]=0;
            LOG(Loglevel::DEBUG)<<"read success message is:"<<buffer;
        }
        else if(n==0){
            //连接退出
            LOG(Loglevel::DEBUG)<<"quit";
            _pfd[pos].fd=defaultfd;
            close(fd);
        }
        else{
            LOG(Loglevel::ERROR)<<"read error";
        }
    }

    void Acceptconnect(){
        InetAddr clie;
        int fd=_listenfd->Accept(&clie);
        if(fd>=0){
            //有新连接到来了
            LOG(Loglevel::DEBUG)<<"new connect";
            int pos=0;
            for(;pos<64;pos++){
                if(_pfd[pos].fd==defaultfd)
                break;
            }
            if(pos==size){
                LOG(Loglevel::DEBUG)<<"error";
            }
            else{
                _pfd[pos].fd=fd;
                _pfd[pos].events=POLLIN;
            }
        }
    }

private:
    std::unique_ptr<Socket> _listenfd;
    bool _isruning;
    struct pollfd _pfd[size];
};


//下面我们来比较poll与select

//1:poll也需要我们去维护一个数组，但是这个数组直接就是select在内核所维护的数组
//由于poll直接将读写事件放在一个结构体中，所以我们不需要每次循环都去遍历并设置新的，节省了一次遍历数组的时间
//也是由内核去遍历数组并填充信息
//支持的文件描述符数量由用户层决定
//每次调用都要把大量的结构体拷贝到内核

