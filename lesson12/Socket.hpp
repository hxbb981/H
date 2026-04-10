#pragma once
#include "Common.hpp"
#include "InetAddr.hpp"
#include "Log.hpp"
using namespace InetAddrMoudel;
using namespace LogMudel;
const int defaultd = -1;
const int gbacklog = 16;

namespace SocketMoudel
{
    class Socket
    {
    public:
        Socket()
        {
        }
        virtual void Create_sock() = 0;
        virtual void Bind_sock(uint16_t port) = 0;
        virtual void listen_sock(int black) = 0;
        virtual std::shared_ptr<Socket> Accept(InetAddr *clie) = 0;
        virtual void Close() = 0;
        virtual int Recv(std::string *buffer) = 0;
        virtual int Send(const std::string &str) = 0;
        //virtual int Connect(InetAddr &addr) = 0;
        virtual int Connect(const std::string &server_ip, uint16_t port)=0;

    public:
        void TcpsockInit(int port, int black = gbacklog)
        {
            Create_sock();
            Bind_sock(port);
            listen_sock(black);
        }
        void Connecty()
        {
            Create_sock();
        }
    };
    class Tcpsocket : public Socket
    {
    public:
        Tcpsocket() : _fd(defaultd) {}
        Tcpsocket(int listenfd) : _fd(listenfd) {} //?没必要吧
        ~Tcpsocket() {}
        void Create_sock() override
        {
            _fd = socket(AF_INET, SOCK_STREAM, 0);
            if (_fd < 0)
            {
                LOG(Loglevel::DEBUG) << "create error";
                exit(SOCK_ERROR);
            }
            LOG(Loglevel::DEBUG) << "create success";
        }
        void Bind_sock(uint16_t port) override
        {
            InetAddr addr(port); // 创建好了套接字
            int m = bind(_fd, addr.Netaddr(), addr.getlen());
            if (m < 0)
            {
                LOG(Loglevel::DEBUG) << "bind error";
                exit(BIND_ERROR);
            }
            LOG(Loglevel::DEBUG) << "bind success";
        }
        void listen_sock(int black) override
        {
            int m = ::listen(_fd, black);
            if (m < 0)
            {
                LOG(Loglevel::DEBUG) << "listen error";
                perror("listen error");
                exit(LISTEN_ERROR);
            }
            LOG(Loglevel::DEBUG) << "listen success";
        }
        std::shared_ptr<Socket> Accept(InetAddr *clie) override
        {
            sockaddr_in peer;

            socklen_t len = sizeof(peer);

            int fd = accept(_fd, CONV(peer), &len); // sockaddr* 往里面输入的,const sockaddr*,发送给别人的
            if (fd < 0)
            {
                LOG(Loglevel::DEBUG) << "accept error";
                exit(ACCEPT_ERROR);
            }
            LOG(Loglevel::DEBUG) << "accept success";
            // clie(peer);这里还不允许直接构造

            clie->Setaddr(peer); // 把信息带出去

            return std::make_unique<Tcpsocket>(fd); // 这里我们又建立了一个对象不过对象中的fd是建立连接的fd
        }
        void Close() override
        {
            close(_fd);
        }

        int Recv(std::string *buffer) override
        {
            char str[1024];
            int m = recv(_fd, str, sizeof(str)-1, 0);
            if (m > 0)
            {
                //std::cout<<"dd";
                str[m] = '\0';
                *buffer += str;
            }
            return m;
        }
        int Send(const std::string &buffer) override
        {
            return send(_fd,buffer.c_str(),buffer.size(),0);
        }
        // int Connect(InetAddr &addr) override
        // {
        //     int m = connect(_fd, addr.Netaddr(), addr.getlen());
        //     if (m < 0)
        //     {
        //         perror("connect");
        //     }
        //     return m;
        // }
        int Connect(const std::string &server_ip, uint16_t port) override
        {
            InetAddr server(server_ip, port);
            return ::connect(_fd, server.Netaddr(), server.getlen());
        }

    private:
        int _fd; // 这里有可能是listenfd和sockfd
    };

    class Udpsocket : public Socket
    {
    };

}