#include"common.hpp"
//客户端，我们只需要读就可以了，如果服务端不打开，客户端也将处于阻塞状态
int main(){
    fifoopener client(PATH,Name);
    client.openforread();
    client.Read();
    client.Close();
}