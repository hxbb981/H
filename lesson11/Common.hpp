
#include<functional>
#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<strings.h>
#include<sys/wait.h>
#include<memory>

enum ExitCode{
    OK=0,
    SOCK_ERROR,
    BIND_ERROR,
    LISTEN_ERROR,
    ACCEPT_ERROR,
    FORK_ERROR,
    CIN_ERROR,
    USE_ERROR,
    CON_ERROR
};
// enum class Exitcode{
//     aaa,
//     bbb
// };//访问内部成员时必须指定命名空间


#define CONV(addr) ((struct sockaddr*)&addr)

//直接弄成全局的