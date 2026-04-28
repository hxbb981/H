
#include"Epollserver.hpp"
#include<iostream>

int main(int argc,char*argv[]){
    if(argc!=2){
        LOG(Loglevel::DEBUG)<<"errorr";
    }
    uint16_t port=std::stoi(argv[1]);

    std::unique_ptr<Epollserver> s=std::make_unique<Epollserver>(port);

    s->Start();
}

