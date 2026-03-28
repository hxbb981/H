#include<iostream>
#include <netinet/in.h>
#include<strings.h>
#include <arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
int main(int argc,char*argv[]){
    if(argc!=3){
        std::cout<<"执行错误"<<std::endl;
        return 1;
    }
    uint16_t serve_port=std::stoi(argv[2]);
    std::string serve_ip=argv[1];
    //创建套接字
    int socketfd=socket(AF_INET,SOCK_DGRAM,0);
    //这个不用绑定，因为ip与端口号都是内置的
    if(socketfd<0){
        std::cout<<"fail"<<std::endl;
        return 2;
    }
    //处理传出来的信息
    struct sockaddr_in server;
    //初始化
    bzero(&server,sizeof(server));
    //填充字段
    server.sin_family=AF_INET;
    server.sin_port=htons(serve_port);
    server.sin_addr.s_addr=inet_addr(serve_ip.c_str());
    //发送消息
    while(true){
        std::string input;
        std::cout<<"请输入"<<std::endl;
        getline(std::cin,input);
        sendto(socketfd,input.c_str(),input.size(),0,(const sockaddr*)&server,sizeof(server));
        //接收数据
        char buffer[1024];
        struct sockaddr_in peer;
        socklen_t len=sizeof(peer);
        int m=recvfrom(socketfd,buffer,sizeof(buffer),0,(struct sockaddr*)&peer,&len);
        if(m>0){
            buffer[m]='\0';
            std::cout<<buffer<<std::endl;
        }
    }
    return 0;
}