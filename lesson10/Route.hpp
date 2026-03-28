#include<iostream>
#include"InetAddr.hpp"
#include<vector>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include"Log.hpp"
using namespace InetAddrMoudel;
using namespace LogMudel;


class Route{
private:
    bool isexit(InetAddr&peer){
        for(auto &user:_admin){
            if(user==peer)return true;
            break;
        }
        return false;
    }
    void adduser(InetAddr&peer){
        LOG(Loglevel::DEBUG)<<"新增一个用户";
        _admin.emplace_back(peer);
    }
    void deleteuser(InetAddr&peer){
        for(auto iter=_admin.begin();iter!=_admin.end();iter++){
            if(*iter==peer){
                LOG(Loglevel::DEBUG)<<"删除一个user";
                _admin.erase(iter);//erase参数需要传迭代器，所以用迭代器遍历
                break;
            }
        }
    }
public:
    Route(){

    }
    ~Route(){

    }
    void MessageRoute(int sockfd,const std::string&message,InetAddr&peer){
        if(!isexit(peer)){
            adduser(peer);
        }
        //将消息发给所有人
        for(auto&ad:_admin){
            int n=sendto(sockfd,message.c_str(),message.size(),0,(const sockaddr*)&ad.Getaddr(),sizeof(ad.Getaddr()));
        }
        if(message=="QUIT"){
            LOG(Loglevel::DEBUG)<<"删除一个用户";
            deleteuser(peer);
        }
    }
private:
    std::vector<InetAddr> _admin;
};