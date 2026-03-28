#include"Log.hpp"
#include"Mutex.hpp"
#include"InetAddr.hpp"
#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<strings.h>
#include"sys/wait.h"

enum ExitCode{
    OK=0,
    SOCK_ERROR,
    BIND_ERROR,
    LISTEN_ERROR,
    ACCEPT_ERROR,
    FORK_ERROR
};


#define CONV(addr) ((struct sockaddr*)&addr)

//直接弄成全局的