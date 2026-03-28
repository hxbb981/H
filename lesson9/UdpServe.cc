#include"UdpServe.hpp"//网络通信功能
#include<memory>
#include"Dict.hpp"//翻译功能
using namespace DictMudle;
std::string task(const std::string&s){
    return s;
}
int main(int args,char *argv[]){
    using namespace UdpServeMoudel;
    uint16_t port=std::stoi(argv[1]);
    Dict dict;
    dict.downword();
    std::unique_ptr<UdpServe> user=std::make_unique<UdpServe>(port,[&dict](const std::string english,InetAddr&clie)->std::string{
        return dict.translate(english,clie);
    });//还需要知道服务器的端口号和ip
    user->Init();//创建服务器
    user->Start();//启动服务器
}