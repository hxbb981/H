#include "Common.hpp"
#include "Log.hpp"
#include "InetAddr.hpp"
#include "Thread.hpp"
#include "Threadpool.hpp"
using namespace LogMudel;
using namespace InetAddrMoudel;
using namespace ThreadpoolMoudel;
using namespace ThreadMoudel;
// using task_t=std::function<void()>;
using task_t = std::function<std::string(const std::string&, InetAddr& addr)>;

const int defaulte = -1;

class TcpServer
{
public:
    TcpServer(uint16_t port, task_t func)
        : _port(port), _socklisfd(defaulte), _isruning(false), _func(func)
    {
    }
    class ThreadDate
    {
    public:
        ThreadDate(int sockfd, const InetAddr &a, TcpServer *t)
            : _sockfd(sockfd), addr(a), T(t)
        {
        }
        int _sockfd;
        InetAddr addr;
        TcpServer *T;
    };
    void server(int sockfd, InetAddr& addr)
    {
        char buffer[1024];
        while (true)
        {
            // 先读
            // m>0 读取成功
            // m<0 读取失败
            // m=0 断开连接 读到文件末尾
            ssize_t m = read(sockfd, buffer, sizeof(buffer)-1);
            if (m > 0)
            {
                buffer[m]='\0';
                LOG(Loglevel::DEBUG) << "read success";
                std::cout << buffer << std::endl;
                //回调处理
                std::string enco=_func(buffer,addr);
                // 写回数据
                write(sockfd,enco.c_str(),enco.size());
            }
            else if (m == 0)
            {
                LOG(Loglevel::DEBUG) << "disconnect";
                close(sockfd);
                break;
            }
            else
            {
                LOG(Loglevel::DEBUG) << "read fail";
                close(sockfd);
                break;
            }
        }
    }
    static void *Route(void *args)
    {
        // 函数返回值必须是void*
        // 用一个类来包装真是恰到好处
        // 直接将线程设置成分离状态，不用等待,并发
        pthread_detach(pthread_self());
        ThreadDate *tp = static_cast<ThreadDate *>(args);
        tp->T->server(tp->_sockfd, tp->addr);
        delete tp;
        return nullptr;
    }

    void Init()
    {
        // 创建套接字
        // signal(SIGCHLD, SIG_IGN);//第一个参数是子进程死了会给父进程这个信号，SIG_IGN是忽略函数，告诉操作系统内核去处理进程，推荐做法
        _socklisfd = socket(AF_INET, SOCK_STREAM, 0);
        if (_socklisfd < 0)
        {
            LOG(Loglevel::FATAL) << "create fail";
            exit(SOCK_ERROR);
        }
        LOG(Loglevel::DEBUG) << "create success";
        // 绑定
        InetAddr addr(_port);
        int m = bind(_socklisfd, (const sockaddr *)&addr.Getaddr(), sizeof(addr.Getaddr()));
        if (m < 0)
        {
            LOG(Loglevel::DEBUG) << "bind fail";
            exit(BIND_ERROR);
        }
        LOG(Loglevel::DEBUG) << "bind success";
        // 将方式设置成listen状态
        int n = listen(_socklisfd, 8);
        if (n < 0)
        {
            LOG(Loglevel::DEBUG) << "listen fail";
            exit(LISTEN_ERROR);
        }
        LOG(Loglevel::DEBUG) << "listen success";
    }
    void Start()
    {
        _isruning = true;
        while (_isruning)
        {
            // 获取连接
            struct sockaddr_in clie;
            socklen_t len = sizeof(sockaddr_in);
            int sockfd = accept(_socklisfd, CONV(clie), &len); // 这里的clie就是客户端的ip和端口号
            if (sockfd < 0)
            {
                LOG(Loglevel::DEBUG) << "accept fail";
                exit(ACCEPT_ERROR);
            }
            LOG(Loglevel::DEBUG) << "accept success";
            InetAddr addr(clie);

            // 方式四，线程池方式，收到消息就调队列，处理

            // 就这样调用
            //  Threadpool<task_t>::Getinstance()->Eqeue([this,&sockfd,&addr](){
            //      this->server(sockfd,addr);
            //  });

            // 方式三：我们采用多线程的方式
            ThreadDate *p = new ThreadDate(sockfd, addr, this);

            pthread_t pid;
            // // 这里的route函数必须是全局或static函数，我们把route定义为static,但是在类内的静态成员函数不能访问非静态函数
            // // 所以我们直接传this指针,还不行
            pthread_create(&pid, nullptr, Route, p); // 我们选择在route函数中去调用receve函数

            // 这里我们就又有问题了，我们不需要等待线程吗？？又是串行？

            // 方式一：
            // server(sockfd,addr);这里效率太低了
            // 所有服务器只能排队来连接，只能一个进程对一个进程
            // 这里我们采用多进程的方式
            // 方式二：
            //     int pid=fork();
            //     if(pid<0){
            //         LOG(Loglevel::ERROR)<<"fork error";
            //         exit(FORK_ERROR);
            //     }
            //     else if(pid==0){
            //         //子进程，是能够看到父进程的sockfd的，因为继承了父进程的虚拟地址空间
            //         //第二种做法
            //         close(_socklisfd);//这里关闭文件描述符是干嘛？
            //         //只负责通信
            //         //子进程会继承父进程的文件描述符表，这里他已经获取了通信的文件，不在需要监听的文件，我们可以把不需要的文件符关闭掉

            //         if(fork()>0){
            //             exit(OK);//这里子进程直接执行退出，但是孙子进程没退出，不会造成僵尸进程吗？
            //             //不会，因为父进程退出后变成孤儿进程，执行完后直接被系统回收
            //         }
            //         server(sockfd,addr);//孙子进程去执行这个函数
            //         exit(OK);
            //     }
            //     else{
            //         //父进程，只能够等待子进程如果不等待则会导致成为僵尸进程
            //         //这样写并没有出现并发的情况，但是父进程又不得不在这等待
            //         pid_t rid=waitpid(pid,nullptr,0);
            //         //这里子进程直接退出无需等待，父进程一直进行循环去创造子进程
            //         //父子进程中如果父进程先退出，子进程变成孤儿进程，执行完毕后直接被系统回收，不会造成僵尸进程
            //         close(sockfd);
            //         //这里关闭文件描述符是干嘛？
            //         //不需要负责通信的文件描述符
            //         (void)rid;
            //     }
            // }
        }
    }

private:
    int _port;
    int _socklisfd;
    bool _isruning;
    task_t _func;
};