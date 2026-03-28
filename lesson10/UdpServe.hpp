#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Log.hpp"
#include "InetAddr.hpp"

using namespace LogMudel;
using namespace InetAddrMoudel;

using func_t = std::function<void(int sockfd, const std::string &, InetAddr &)>;

const int defaultfd = -1;

// 你是为了进行网络通信的！
class UdpServer
{
public:
    UdpServer(uint16_t port, func_t func)
        : _sockfd(defaultfd),
          //   _ip(ip),
          _port(port),
          _isrunning(false),
          _func(func)
    {
    }
    void Init()
    {
        // 1. 创建套接字
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
             LOG(Loglevel::FATAL) << "socket error!";
            exit(1);
        }
        // LOG(Loglevel::INFO) << "socket success, sockfd : " << _sockfd;

        // 2. 绑定socket信息，ip和端口， ip(比较特殊，后续解释)
        // 2.1 填充sockaddr_in结构体
        struct sockaddr_in local;
        bzero(&local, sizeof(local));
        local.sin_family = AF_INET;
        // 我会不会把我的IP地址和端口号发送给对方？
        // IP信息和端口信息，一定要发送到网络！
        // 本地格式->网络序列
        local.sin_port = htons(_port);
        // IP也是如此，1. IP转成4字节 2. 4字节转成网络序列 -> in_addr_t inet_addr(const char *cp);
        // local.sin_addr.s_addr = inet_addr(_ip.c_str()); // TODO
        local.sin_addr.s_addr = INADDR_ANY;

        // 那么为什么服务器端要显式的bind呢？IP和端口必须是众所周知且不能轻易改变的！
        int n = bind(_sockfd, (struct sockaddr *)&local, sizeof(local));
        if (n < 0)
        {
            LOG(Loglevel::FATAL) << "bind error";
            exit(2);
        }
        // LOG(Loglevel::INFO) << "bind success, sockfd : " << _sockfd;
    }
    void Start()
    {
        _isrunning = true;
        while (_isrunning)
        {
            char buffer[1024];
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            // 1. 收消息, client为什么要个服务器发送消息啊？不就是让服务端处理数据。
            ssize_t s = recvfrom(_sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&peer, &len);
            if (s > 0)
            {
                InetAddr client(peer);
                buffer[s] = 0;
                // TODO
                _func(_sockfd, buffer, client);

                // LOG(LogLevel::DEBUG) << "[" << peer_ip << ":" << peer_port<< "]# " << buffer; // 1. 消息内容 2. 谁发的？？

                // 2. 发消息
                // std::string echo_string = "server echo@ ";
                // echo_string += buffer;
                // sendto(_sockfd, result.c_str(), result.size(), 0, (struct sockaddr*)&peer, len);
            }
        }
    }
    ~UdpServer()
    {
    }

private:
    int _sockfd;
    uint16_t _port;
    // std::string _ip; // 用的是字符串风格，点分十进制, "192.168.1.1"
    bool _isrunning;

    func_t _func; // 服务器的回调函数，用来进行对数据进行处理
};