#include"Common.hpp"
#include"Log.hpp"
#include"InetAddr.hpp"
using namespace LogMudel;
using namespace InetAddrMoudel;
int main(int argc,char*argv[]){
    if(argc!=3){
        LOG(Loglevel::ERROR)<<"use error";
        exit(USE_ERROR);
    }

    std::string peer_ip=argv[1];
    uint16_t port=std::stoi(argv[2]);

    //创建套接字
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        LOG(Loglevel::ERROR)<<"socket error";
        exit(SOCK_ERROR);
    }

    //绑定，这里我们不需要绑定，系统会自己绑定

    //建立连接,此时我们的序列还是主机序列

    InetAddr peer(peer_ip,port);//创建对应的结构体

    int n=connect(sockfd,(const sockaddr*)&peer.Getaddr(),sizeof(peer.Getaddr()));
    if(n<0){
        LOG(Loglevel::WARNING)<<"connect error";
        exit(CON_ERROR);
    }


    LOG(Loglevel::DEBUG)<<"connect success";


    while(true){
        std::cout<<"please enter"<<std::endl;

        std::string message;

        getline(std::cin,message);
        int n=write(sockfd,message.c_str(),message.size());

        char buffer[1024];
        ssize_t m=read(sockfd,buffer,sizeof(buffer)-1);
        if(m>0){
            buffer[m]='\0';
            std::cout<<buffer<<std::endl;
        }
    }
    return 0;
}