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
#include <poll.h>
#include <vector>

void pollTcp(int fd, int revents){
    if (revents & POLLIN){
        char buff[255]="";
        read(fd, buff, sizeof(buff));
        printf("tcp: %s\n", buff);
    }
}

void pollUdp(int fd, int revents){
    if(revents & POLLIN){
        char buff[255]="";
        read(fd, buff, sizeof(buff));
        printf("udp: %s\n", buff);
    }
}

int main(int argc, char ** argv){
    long port = std::stol(argv[1]);
    sockaddr_in myAddr{};
    myAddr.sin_family = AF_INET;
    myAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    myAddr.sin_port = htons((uint16_t)port);
    int tcpfd = socket(AF_INET, SOCK_STREAM, 0);
    int udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(tcpfd == -1||udpfd == -1){
        perror("socket failed");
        exit(1);
    }
 
    int fail = bind(tcpfd, (sockaddr*) &myAddr, sizeof(myAddr));
    int fail2 = bind(udpfd, (sockaddr*) &myAddr, sizeof(myAddr));
    if(fail||fail2){
        perror("bind failed");
        exit(1);
    }

    long sport = std::stol(argv[2]);
    sockaddr_in servAddr{};
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons((uint16_t)sport);
    int c = connect(tcpfd, (sockaddr*)&servAddr, sizeof(servAddr));
    printf("%d\n", c);
    pollfd descr[2];
    descr[0].fd=tcpfd;
    descr[0].events=POLLIN;
    descr[1].fd=udpfd;
    descr[1].events=POLLIN;
    while(true){

        int ready = poll(descr, 2, -1);
            if(ready == -1){
                std::cout << "poll error"<<std::endl;
                exit(-1);
            }
            for(long unsigned int i = 0 ; i < 2 && ready > 0 ; ++i){
                if(descr[i].revents){
                    if(descr[i].fd == tcpfd)
                        pollTcp(tcpfd, descr[0].revents);
                    else
                        pollUdp(udpfd, descr[1].revents);
                    ready--;
            }
        }
    }
}