#include"task.hpp"
#include"Blockqueue.hpp"
void*consumer(void*args){
    Blockqueue<func_t>* b=static_cast<Blockqueue<func_t>*>(args);
    while(1){
        func_t t=b->Top();

        t();
    }
}
void *productor(void*args){
    Blockqueue<func_t>*b=static_cast<Blockqueue<func_t>*>(args);
    while(true){
        b->Equeue(downtask);
    }
}
int main(){
    Blockqueue<func_t>*b=new Blockqueue<func_t>();
    pthread_t c[2],p[3];
    pthread_create(c,nullptr,consumer,b);
    pthread_create(c+1,nullptr,consumer,b);
     pthread_create(p, nullptr, productor, b);
    pthread_create(p+1, nullptr, productor, b);
    pthread_create(p+2, nullptr, productor, b);

    pthread_join(c[0], nullptr);
    pthread_join(c[1], nullptr);
    pthread_join(p[0], nullptr);
    pthread_join(p[1], nullptr);
    pthread_join(p[2], nullptr);
    return 0;


}