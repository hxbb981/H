
#include"selectserver.hpp"
#include<iostream>

int main(int argc,char*argv[]){
    if(argc!=2){
        LOG(Loglevel::DEBUG)<<"errorr";
    }
    uint16_t port=std::stoi(argv[1]);

    std::unique_ptr<selectserver> s=std::make_unique<selectserver>(port);

    s->Satrt();






}