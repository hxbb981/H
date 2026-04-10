#include"TcpServer.hpp"
#include"Protocol.hpp"
#include"Netcal.hpp"


int main(int argc,char*argv[]){
    if(argc!=2){
        LOG(Loglevel::DEBUG)<<"cin error";
    }

    uint16_t port=std::stoi(argv[1]);
    
    std::unique_ptr<Netcal>n=std::make_unique<Netcal>();//应用层



    // std::unique_ptr<Protocol>p=std::make_unique<Protocol>([&n](Request&req)->Response{
    //     return n->solve(req);
    // });//协议层

     // 2. 协议层
    std::unique_ptr<Protocol> p = std::make_unique<Protocol>([&n](Request &req)->Response{
        return n->solve(req);
    });




    std::unique_ptr<TcpServer>q=std::make_unique<TcpServer>(port,[&p](std::shared_ptr<Socket>&s, InetAddr &addr){
        p->GetRequest(s,addr);

    });//网络层

    q->Start();//这里才开始进行listen并创建fd



    return 0;
}