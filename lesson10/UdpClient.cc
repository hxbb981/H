#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include "Log.hpp"
#include "Thread.hpp"
using namespace ThreadMoudel;
using namespace LogMudel;
int sockfd = 0; // 设置成全局的，各个线程都可以看到
std::string serve_ip;
int serve_port = 0;

// 发送
void Send()
{
    // 创建结构体并赋值
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(serve_port);
    server.sin_addr.s_addr = inet_addr(serve_ip.c_str());
    std::cout<<"please enter"<<std::endl;
    std::string in;
    getline(std::cin,in);
    //int m = sendto(sockfd, &in, sizeof(in), 0, (const sockaddr *)&server, sizeof(server));
    int m = sendto(sockfd, in.c_str(), in.size(), 0, (const sockaddr *)&server, sizeof(server));
    while (true)
    {
        std::string input;
        getline(std::cin, input);
        int m = sendto(sockfd, &input, sizeof(input), 0, (const sockaddr *)&server, sizeof(server));
        (void)m;
    }
}
// 接受
void Recv()
{
    // 创建结构体来接受服务端的ip和端口号
    while (true)
    {
        struct sockaddr_in server;
        bzero(&server, sizeof(server));
        char buffer[1024];
        socklen_t len;
        int m = recvfrom(sockfd, &buffer, sizeof(buffer), 0, (struct sockaddr *)&server, &len);
        if (m > 0)
        {
            buffer[m] = '\0';
            std::cout << buffer << std::endl;
        }
    }
}
int main(int argc, char *argv[])
{
    // 创建套接字
    if (argc != 3)
    {
        LOG(Loglevel::DEBUG) << "执行错误";
    }
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // 自动绑定端口号
    // 获取服务端的ip和端口号
    serve_ip = argv[1];
    serve_port = std::stoi(argv[2]);

    // 创建两个线程，一个发送，一个接受
    Thread send(Send);
    Thread rece(Recv);

    send.Start();
    rece.Start();

    send.Join();
    rece.Join();

    return 0;
}