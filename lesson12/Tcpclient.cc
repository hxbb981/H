#include"Log.hpp"
#include"InetAddr.hpp"
#include"Socket.hpp"
#include"Protocol.hpp"

using namespace LogMudel;
using namespace InetAddrMoudel;
using namespace SocketMoudel;


void Strucinit(int &x,int&y,char&oper){
    std::cout<<"enter x"<<std::endl;
    std::cin>>x;
    std::cout<<"enter y"<<std::endl;
    std::cin>>y;
    std::cout<<"enter oper"<<std::endl;
    std::cin>>oper;
}


int main(int argc,char*argv[]){
    if(argc!=3){
        LOG(Loglevel::DEBUG)<<"enter error";
    }
    uint16_t port=std::stoi(argv[2]);

    std::string server_ip=argv[1];

    InetAddr addr(server_ip,port);

    std::shared_ptr<Socket> c=std::make_unique<Tcpsocket>();//网络套接字

    c->Connecty();

    if(c->Connect(server_ip,port)!=0){
        LOG(Loglevel::DEBUG)<<"connect error";
        exit(CON_ERROR);
    }
    LOG(Loglevel::DEBUG)<<"connect success";

    std::unique_ptr<Protocol>p=std::make_unique<Protocol>();//序列化

    std::string message;

    while(true){
        int x,y;char oper;
        Strucinit(x,y,oper);

        std::string send_ptr=p->sendrequest(x,y,oper);

        //std::cout<<send_ptr<<std::endl;

        c->Send(send_ptr);

        //获取应答
        Response rese;

        p->Getresponse(c,message,&rese);

        rese.Showresult();
    }

}