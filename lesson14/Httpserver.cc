
#include"Http.hpp"

int main(int argc,char*argv[]){
    if(argc!=2){
        LOG(Loglevel::DEBUG)<<"user error";
        exit(USE_ERROR);
    }
    
    uint16_t port=std::stoi(argv[1]);


    //创造服务器，在http中构建
    std::unique_ptr<Http> http=std::make_unique<Http>(port);

    http->Start();




}