#pragma once
#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
namespace InetAddrMoudel{
    class InetAddr{
        public:
            InetAddr(struct sockaddr_in peer)
            :_addr(peer)
            {
                //给结构体转序列
                _port=ntohs(peer.sin_port);
                _ip=inet_ntoa(peer.sin_addr);
            }
            uint16_t Port(){
                return _port;
            }
            std::string Ip(){
                return _ip;
            }
            bool operator==(InetAddr&add){
                return _port==add._port&&_ip==add._ip;
            }
            const struct sockaddr_in& Getaddr(){
                return _addr;
            }
            ~InetAddr(){

            }
        private:
            uint16_t _port;
            std::string _ip;
            struct sockaddr_in _addr;
    };
}
