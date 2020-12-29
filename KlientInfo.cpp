#include "KlientInfo.h"

KlientInfo::KlientInfo(int f, sockaddr_in c){
    fd = f;
    clientAddr = c;
    clientAddrSize = sizeof(clientAddr);
}
int KlientInfo::getFd(){
    return fd;
}
sockaddr_in* KlientInfo::getAddr(){
    return &clientAddr;
}
socklen_t KlientInfo::getSize(){
    return clientAddrSize;
}