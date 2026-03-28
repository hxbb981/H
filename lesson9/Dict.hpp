#include<iostream>
#include<string>
#include<unordered_map>
#include<fstream>
#include"Log.hpp"
#include"InetAddr.hpp"
const std::string p="./dictionary.txt";
const std::string ch=":";
namespace DictMudle{
    using namespace LogMudel;
    using namespace InetAddrMoudel;
    class Dict{
        public:
            Dict(const std::string path=p):
                _path(path)
            {}
            bool downword(){
                //首先需要打开文件
                std::ifstream in(_path);
                if(!in.is_open()){
                    return false;
                }
                std::string line;
                while(getline(in,line)){
                    std::string chinese;
                    std::string english;
                    if(line.empty())continue;
                    int pos=line.find(ch);
                    if(pos==std::string::npos)continue;
                    chinese=line.substr(0,pos);
                    english=line.substr(pos+=ch.size());
                    _dict.insert(std::make_pair(chinese,english));
                }
                in.close();
                return true;
            }
            const std::string translate(const std::string english,InetAddr&client){
                LOG(Loglevel::DEBUG);
                auto pos=_dict.find(english);
                if(pos==_dict.end()) return std::string("unknown");
                else{
                    LOG(Loglevel::DEBUG)<<"进入翻译模块"<<client.Port()<<":"<<client.Ip();
                    return pos->second;
                } 
            }
        private:
            std::string _path;
            std::unordered_map<std::string,std::string> _dict;
    };
}