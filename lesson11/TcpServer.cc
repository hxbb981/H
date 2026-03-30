
#include"TcpServer.hpp"
#include"Dict.hpp"
#include"Command.hpp"
using namespace DictMudle;
int main(int argc,char*argv[]){
    if(argc!=2){
        LOG(Loglevel::ERROR)<<"cin error";
        exit(CIN_ERROR);
    }
    uint16_t port=std::stoi(argv[1]);

    //创建服务器

    //接下来我们将翻译的接口拿过来
    // Dict d;
    // d.downword();

    //把命令的模块拿下来
    Command c;
    std::unique_ptr<TcpServer> tp=std::make_unique<TcpServer>(port,[&c](const std::string& a,InetAddr&clie)->std::string{
        return c.Execute(a,clie);
    });
    // std::unique_ptr<TcpServer> tp=std::make_unique<TcpServer>(port,[&d](const std::string& english,InetAddr& clie){
    //     return d.translate(english,clie);
    // });

    //开始服务器

    tp->Init();

    tp->Start();
    



}