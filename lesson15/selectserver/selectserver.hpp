#include"Socket.hpp"
#include"Common.hpp"


using namespace SocketMoudel;
using namespace LogMudel;
const int defaultfd=-1;
class selectserver{

public:
    selectserver(uint16_t port)
    :_isruning(false)
    ,_listenfd(std::make_unique<Tcpsocket>())
    {
        _listenfd->TcpsockInit(port);
        //初始化记录数组
        for(int i=0;i<64;i++){
            _afd[i]=defaultfd;
        }
        _afd[0]=_listenfd->Fd();
    }

    void Satrt(){
        _isruning=true;
        while(_isruning){
            //1:我们这里不要进行accept 因为等 下面我们来认识一下select接口
            //每一次循环之前，我们都要遍历数组，将需要监听的文件描述符写入位图结构中
            fd_set rfds;
            FD_ZERO(&rfds);
            int maxfd=defaultfd;
            for(int i=0;i<64;i++){
                if(_afd[i]!=defaultfd){
                    FD_SET(_afd[i],&rfds);
                    maxfd=_afd[i];
                }
                else break;
            }
            // FD_ZERO(&rfds);
            // FD_SET(_listenfd->Fd(),&rfds);//将listen套接字接入到位图结构中

            //第一个参数是用户告诉内核需要给我监管的文件描述符中的最大值+1
            //第二个是内核定义的结构体，这个结构体就是一个位图结构/数组,下标就是文件描述符,而内容就是0或一
            //是用户告诉内核是否需要帮助我监管这个文件描述符
            //第三,四个跟第二个一样,分别表示关心读事件,写事件,异常事件。
            //其原理就是用户吧数据结构交给内核，内核遍历，创建新的数据结构，并设置数据结构返回给用户，所以他们既是输出型参数
            //也是输入型参数
            //第四个参数也是一个结构体，可以用来设置时间，如果为nullptr表示一直阻塞等待，如果为0，表示一直非阻塞等待
            //如果设置了时间，表示在这个时间段内阻塞等待，超过时间非阻塞等待
            //如果函数返回0表示等待超时没有就绪
            //如果大于0，表示状态改变的文件描述符的个数
            //如果小于0，表示等待出错了
            timeval timeout;
            timeout.tv_sec=2;//设置s
            timeout.tv_usec=2;//设置us
            //当我们每次调用select的时候，我们的位图结构都会被改变，需要关注的文件描述符就丢失了，所以select需要一个辅助数组
            //当我们每次获取新连接的时候，记录文件描述符
            int n=select(maxfd+1,&rfds,nullptr,nullptr,&timeout);
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
                    Dispatch(rfds);//处理事件
                    break;
            }
        }
    }

    void Dispatch(fd_set& rfd){
        for(int i=0;i<64;i++){
            if(FD_ISSET(_afd[i],&rfd)){
                //表示自己所要查找的文件描述符是否就绪，也就是是否在位图结构中
                //如果在返回值大于0，不在为0
                if(_afd[i]==_listenfd->Fd()){

                    //则表明新连接到来了
                    Acceptconnect();
                }
                else{
                    //表示文件描述符的读事件就绪
                    Recv(_afd[i],i);

                }
            }
        }
    }
    void Recv(int fd,int pos){
        //从缓冲区中取数据
        char buffer[1024];
        ssize_t n=recv(fd,buffer,sizeof(buffer),0);
        if(n>0){
            LOG(Loglevel::DEBUG)<<"read success message is:"<<buffer;
        }
        else if(n==0){
            //连接退出
            LOG(Loglevel::DEBUG)<<"quit";
            _afd[pos]=defaultfd;
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
                if(_afd[pos]==defaultfd)
                break;
            }
            if(pos==64)LOG(Loglevel::ERROR)<<"error";
            else _afd[pos]=fd;
        }
    }

private:
    std::unique_ptr<Socket> _listenfd;
    bool _isruning;
    int _afd[64];
};


//下面我们来了解select的缺点

//1，需要自己手动创造一个辅助数组
//2，每一次循环时，都要遍历数组并构建性的集合
//3，每次设置新的集合，内核都会在底层遍历检查，效率低下
//4，当有事件就绪时，我们又需要遍历数组来检测，又是循环
//5，select用的是fd_set集合，支持的文件描述符是有上限的
