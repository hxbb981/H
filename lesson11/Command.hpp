#include<iostream>
#include<vector>
#include<string>
#include"InetAddr.hpp"
using namespace InetAddrMoudel;


class Command{
public:
    Command(){
        white.push_back("ls");
        white.push_back("pwd");
        white.push_back("ll");
    }
    bool check(const std::string command){
        for(auto it=white.begin();it!=white.end();it++){
            if(*it==command)return true;
        }
        return false;
    }
    std::string Execute(const std::string command,InetAddr &addr){
        //判断命令是否在白名单中
        if(!check(command)){
            return std::string("huai ren");
        }
        std::string person=addr.string_addr();
        //执行命令

        FILE*f=popen(command.c_str(),"r");

        if(f==nullptr){
            return std::string("no command");
        }
        std::string res;
        char line[1024];
        while(fgets(line,sizeof(line),f)){
            res+=line;
        }
        pclose(f);
        std::string result=person+res;
        return result;
    }



private:
    std::vector<std::string> white; 





};