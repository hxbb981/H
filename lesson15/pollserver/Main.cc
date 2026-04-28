
#include"Pollserver.hpp"
#include<iostream>

int main(int argc,char*argv[]){
    if(argc!=2){
        LOG(Loglevel::DEBUG)<<"errorr";
    }
    uint16_t port=std::stoi(argv[1]);

    std::unique_ptr<Pollserver> s=std::make_unique<Pollserver>(port);

    s->Start();
}

