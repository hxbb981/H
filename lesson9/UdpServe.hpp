#include <iostream>
#include<string>
#include<strings.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<functional>
#include"InetAddr.hpp"
namespace UdpServeMoudel
{
    using namespace InetAddrMoudel;
    using func_t=std::function<std::string(const std::string&,InetAddr&)>;
    using namespace InetAddrMoudel;
    class UdpServe
    {
    public:
        UdpServe(uint16_t port,func_t func)
        :_port(port)
        //,_ip(ip)
        ,_func(func)
        {}
        void Init(){
            //主要目的是创建套接字并绑定自己的信息
            //1:创建套接字
            _socketfd=socket(AF_INET,SOCK_DGRAM,0);
            //socket为创建套接字函数，第一个参数表示使用网络通信，第二个参数表示使用Udp(面向数据报)，第三个参数，默认设置成0
            if(_socketfd<0){
                exit(1);
            }
            //2:我们需要绑定自己的ip和端口号
            //2.1：创建sockaddr_in结构体，结构体中包括协议，port和ip
            struct sockaddr_in lock;
            bzero(&lock,sizeof(lock));//将结构体的成员全部初始化成0
            lock.sin_family=AF_INET;//网络协议
            //我们服务器在发送信息时需要将自己的ip和port发送出去，但由于各个机器的网络序列不同
            //而向网络中发送信息时必须是大端的网络序列，所以我们需要将数据转化成大端
            lock.sin_port=htons(_port);
            lock.sin_addr.s_addr = htonl(INADDR_ANY);;//将数据转化成大端并初始化结构体，绑定ip地址为0，这样可以保证只要是访问这台机器的端口号，都能接收到数据
            //2.2开始绑定
            int n=bind(_socketfd,(const sockaddr*)&lock,sizeof(lock));
            if(n<0){
                exit(2);
            }
            (void)n;
        }
        void Start(){
            //服务器是一直处于工作状态的
            _isruning=true;
            while(_isruning){
                //1：接收到客户端发来的消息
                //recvfrom函数，1：套接字描述符：谁来接收这个文件 int
                //2：void* ：接受信息的缓冲区 3：size_t 缓冲区大小 4：int 默认是0 5：sockaddr_in 输入型参数，将客户端的信息接受
                //6：socklen_t 结构体大小
                char buffer[1024];
                struct sockaddr_in peer;
                socklen_t len = sizeof(SO_PREFER_BUSY_POLL);
                ssize_t s=recvfrom(_socketfd,buffer,sizeof(buffer)-1,0,(sockaddr*)&peer,&len);//ssize_t也就是long int，也就是数据
                if(s>0){
                    //1:处理接收到的信息

                    buffer[s] = '\0';
                    // uint16_t peer_port=ntohs(peer.sin_port);//从网络中的字节序列转化到服务器序列
                    // std::string peer_ip=inet_ntoa(peer.sin_addr);
                    InetAddr addr(peer);
                    std::cout<<buffer<<std::endl;
                    //2：发消息
                    std::string result=_func(buffer,addr);
                    //std::string str;
                    //getline(std::cin,str);
                    //len = sizeof(peer);
                    int n = sendto(_socketfd,result.c_str(),result.size(),0,(const sockaddr*)&peer,len);
                    std::cout << result.c_str() << " " << result.size() << std::endl;
                    if (n < 0)perror("sendto failed");
                    std::cout << n << std::endl;
                }
            }
        }
        ~UdpServe(){

        }
    private:
        int _socketfd;//打开服务器需要有socket套接字，也就是文件描述符
        uint16_t _port;//记录进程的端口号
        //std::string _ip;//字符串类型来记录ip
        bool _isruning;
        func_t _func;
    };
}
