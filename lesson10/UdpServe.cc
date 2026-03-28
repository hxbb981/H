#include "UdpServe.hpp" //网络通信功
#include <memory>
#include "Route.hpp"
#include"Threadpool.hpp"
using namespace ThreadpoolMoudel;
using task_t=std::function<void()>;
std::string task(const std::string &s)
{
    return s;
}

int main(int args, char *argv[])
{
    uint16_t port = std::stoi(argv[1]);
    // Dict dict;
    // dict.downword();
    //路由功能
    Route r;

    //线程池

    //获取单例
    auto tp=Threadpool<task_t>::Getinstance();


     std::unique_ptr<UdpServer> user = std::make_unique<UdpServer>(port, [&r,&tp](int sockfd, const std::string message, InetAddr &clie){
         task_t t=[&r,&sockfd,&message,&clie](){r.MessageRoute(sockfd,message,clie);};
         tp->Eqeue(t);
     }); // 还需要知道服务器的端口号和ip
    //  std::unique_ptr<UdpServer> user = std::make_unique<UdpServer>(port, [&r,&tp](int sockfd, const std::string message, InetAddr &clie){
    //     task_t t=[&r,&sockfd,&message,&clie](){r.MessageRoute(sockfd,message,clie);};
    //     tp->Eqeue(t);
    // });
    // std::unique_ptr<UdpServer>user=std::make_unique<UdpServer>(port,[&r](int sockfd,const std::string message,InetAddr&clie){
    //     r.MessageRoute(sockfd,message,clie);
    // });
    user->Init();                                                                                              // 创建服务器
    user->Start();                                                                                             // 启动服务器
}