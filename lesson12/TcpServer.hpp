#pragma once
#include"Socket.hpp"
using namespace SocketMoudel;
using func_t = std::function<void(std::shared_ptr<Socket>&s, InetAddr &addr)>;
class TcpServer
{
public:
    TcpServer(uint16_t port, func_t func)

        : _port(port), _isruning(false), _func(func), _listenptr(std::make_unique<Tcpsocket>()) // 构造一个socket指针指向Tcpserver,默认构造
    {
        _listenptr->TcpsockInit(_port); // 在构造是直接完成套接字三件套
    }

    void Start()
    {
        // 首先需要一直接受listen
        _isruning = true;
        while (_isruning)
        {
            InetAddr clie;
            std::shared_ptr<Socket> sockfd = _listenptr->Accept(&clie); // 这里指针指向一个结构体
            // 以上我们就创建套接字并接听成功了
            pid_t id = fork();
            if (id < 0)
            {
                LOG(Loglevel::DEBUG) << "create error";
            }
            else if (id == 0)
            {
                // 子进程
                _listenptr->Close();
                if (fork() > 0)
                    exit(OK);
                //_func(sockfd, clie);
                exit(OK);
            }
            else
            {
                // 父进程需要关闭sockfd
                sockfd->Close();

                int m = waitpid(id, nullptr, 0);
            }
        }
    }

private:
    uint16_t _port;
    bool _isruning;
    std::unique_ptr<Socket> _listenptr;
    func_t _func;
};