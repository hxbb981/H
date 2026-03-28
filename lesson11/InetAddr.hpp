#pragma once
#include "Common.hpp"
namespace InetAddrMoudel
{
    class InetAddr
    {
    public:
        InetAddr(struct sockaddr_in peer)
            : _addr(peer)
        {
            // 网络转主机
            _port = ntohs(peer.sin_port);
            char ipbuffer[64];
            inet_ntop(AF_INET, &peer.sin_addr, ipbuffer, sizeof(ipbuffer));
            _ip=ipbuffer;
        }
        InetAddr(int port)
            : _port(port)
        {
            // 主机转网络
            bzero(&_addr, sizeof(_addr));
            _addr.sin_family = AF_INET;
            _addr.sin_port = htons(_port);
            _addr.sin_addr.s_addr = INADDR_ANY;
        }
        uint16_t Port()
        {
            return _port;
        }
        std::string Ip()
        {
            return _ip;
        }
        bool operator==(InetAddr &add)
        {
            return _port == add._port && _ip == add._ip;
        }
        const struct sockaddr_in &Getaddr()
        {
            return _addr;
        }
        ~InetAddr()
        {
        }

    private:
        uint16_t _port;
        std::string _ip;
        struct sockaddr_in _addr;
    };
}
