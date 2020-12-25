#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <thread>

int main(int argc, char ** argv) {
    
    long port = 12345;
    
    sockaddr_in myAddr {};
    myAddr.sin_family = AF_INET;
    myAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    myAddr.sin_port = htons((uint16_t)port);
    
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    if(fd == -1){
        perror("socket failed");
        return 1;
    }
 
    int fail = bind(fd, (sockaddr*) &myAddr, sizeof(myAddr));
    if(fail){
        perror("bind failed");
        return 1;
    }
    
    fail = listen(fd, 1);
    if(fail){
        perror("listen failed");
        return 1;
    }
    
    while(true){
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientFd = accept(fd, (sockaddr*)&clientAddr, &clientAddrLen);
        if(clientFd == -1){
            perror("accept failed");
            return 1;
        }
        else{
            std::thread t([clientAddr, clientFd](){
                printf("Connection from %s:%hu\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
                char buff[255];
                read(clientFd, buff, 255);
                printf("%s/n", buff);
                shutdown(clientFd, SHUT_RDWR);
                close(clientFd);
            });
            std::cout << "po watku"<<std::endl;
        } 
    }
}
