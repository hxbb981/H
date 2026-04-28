#include "Socket.hpp"
#include "Common.hpp"
#include <sys/poll.h>
#include <sys/epoll.h>

using namespace SocketMoudel;
using namespace LogMudel;
const int defaultfd = -1;
const int size = 64;
class Epollserver
{

public:
    Epollserver(uint16_t port)
        : _isruning(false), _listenfd(std::make_unique<Tcpsocket>())
    {
        _listenfd->TcpsockInit(port);
        // 1:创建epoll模型,本质就是os在内核中创建红黑树和就绪队列
        _epfd = epoll_create(256); // 为什么这里的返回值是文件描述符
        if (_epfd < 0)
        {
            LOG(Loglevel::ERROR) << "create error";
            exit(USE_ERROR);
        }
        // 2:将listen套接字插入到内核数据结构上；
        struct epoll_event ev; // 结点类型
        ev.events = EPOLLIN;
        ev.data.fd = _listenfd->Fd();
        // 告诉内核要将一个文件描述符设置到内核的epoll模型中，本质就是在红黑树中进行增删改
        // 其中第二个参数还有很多种例如删除
        // 成功返回0，失败返回-1
        int m = epoll_ctl(_epfd, EPOLL_CTL_ADD, _listenfd->Fd(), &ev);
        if (m < 0)
        {
            LOG(Loglevel::ERROR) << "ctl error";
        }
    }

    void Start()
    {
        _isruning = true;
        while (_isruning)
        {
            //当有事件就绪时直接返回就序列表！！

            int n = epoll_wait(_epfd,_events,size,10000);


            switch (n)
            {
            case 0:
                LOG(Loglevel::ERROR) << "time out";
                break;
            case -1:
                LOG(Loglevel::ERROR) << "select error";
                break;
            default:
                // 有事件就绪了，到底是有新连接到来，还是读事件就绪了
                // 我们当然知道，因为listenfd在这里一定是3
                Dispatch(n); // 处理事件
                break;
            }
        }
    }

    void Dispatch(int num)
    {
        // 遍历
        for(int i=0;i<num;i++){
            int sockfd=_events[i].data.fd;
            uint32_t revents=_events[i].events;//????
            if(revents&EPOLLIN){
                //读肯定就绪了
                if(sockfd==_listenfd->Fd()){
                    Acceptconnect();
                }
                else{
                    Recv(sockfd,i);
                }
            }
        }
    }
    void Recv(int fd, int pos)
    {
        // 从缓冲区中取数据
        char buffer[1024];
        ssize_t n = recv(fd, buffer, sizeof(buffer), 0);
        if (n > 0)
        {
            buffer[n] = 0;
            LOG(Loglevel::DEBUG) << "read success message is:" << buffer;
        }
        else if (n == 0)
        {
            // 连接退出
            LOG(Loglevel::DEBUG) << "quit";
            int m=epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,nullptr);

            close(fd);
        }
        else
        {
            LOG(Loglevel::ERROR) << "read error";
        }
    }

    void Acceptconnect()
    {
        InetAddr clie;
        int fd = _listenfd->Accept(&clie);
        if (fd >= 0)
        {
            // 有新连接到来了
            LOG(Loglevel::DEBUG) << "new connect";
            //添加新的结点就可以了
            struct epoll_event e;
            e.data.fd=fd;
            e.events=EPOLLIN;
            int m=epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&e);
            if(m<0){
                LOG(Loglevel::DEBUG)<<"ctl error";
            }
        }
    }

private:
    std::unique_ptr<Socket> _listenfd;
    bool _isruning;
    int _epfd;
    struct epoll_event _events[size];
};

//下面我们来谈谈epoll的底层原理与他的厉害之处

//创建一个epoll模型时会有一个文件描述符 也就是struct file,里面会有一个private*date的指针，他会指向一个eventpoll结构体
//这个结构体有一个红黑树的头结点指针一个就绪队列的头结点指针，而每一个就绪事件都有一个epoll_item结构体，这个结构体，存在红黑树和就绪队列结点
//eventpoll会指向这两个结点,对于每一个就绪事件，都有一个回调函数，当事件触发时，直接加入到就绪队列中无需遍历，然后直接将就绪队列拷贝到上层


//与select和poll相比的优势

//1poll虽然也是添加结点，但并不是内核所维护，所以每次调用poll时都要进行拷贝
//2：当有就绪事件时，poll需要遍历内核数组，找出fd并拷贝到上层
//
