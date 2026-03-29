
#include"TcpServer.hpp"
#include"Dict.hpp"
using namespace DictMudle;
int main(int argc,char*argv[]){
    if(argc!=2){
        LOG(Loglevel::ERROR)<<"cin error";
        exit(CIN_ERROR);
    }
    uint16_t port=std::stoi(argv[1]);

    //创建服务器

    //接下来我们将翻译的接口拿过来
    Dict d;
    d.downword();


    std::unique_ptr<TcpServer> tp=std::make_unique<TcpServer>(port,[&d](const std::string& english,InetAddr& clie){
        return d.translate(english,clie);
    });

    //开始服务器

    tp->Init();

    tp->Start();
    



}