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

/*
narazie serwer tylko przyjmuje wiadomosci i je wypisuje, ale robi to juz w miare wspolbieznie, wiec gicior, to jest poczatek do dostawania requestow od klienta
*/


std::vector<pollfd> descr;

//obsluga fd serwera
void server(int revents, int servFd){        

    if(revents & ~POLLIN){
        std::cout << errno << "Event "<<revents<<"on server socket"<<std::endl;
        return;
    }
    //dodawanie nowego fd klienta do descr
    if(revents & POLLIN){
        sockaddr_in clientAddr{};
        socklen_t clientAddrSize = sizeof(clientAddr);
        
        auto clientFd = accept(servFd, (sockaddr*) &clientAddr, &clientAddrSize);
        if(clientFd == -1) std::cout<<"accept failed"<<std::endl;
        
        pollfd tmp;
        tmp.fd = clientFd;
        tmp.events = POLLIN|POLLRDHUP;
        descr.push_back(tmp);
        printf("new connection from: %s:%hu (fd: %d)\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), clientFd);
    }
}
//obsluga fd klientow
void client(int indexInDescr) {
    auto clientFd = descr[indexInDescr].fd;
    auto revents = descr[indexInDescr].revents;
    //pobieranie wiadomosci i wyswietlenie jej
    if(revents & POLLIN){
        char buffer[255] = "";
        int count = read(clientFd, buffer, 255);
        if(count < 1)
            revents |= POLLERR;
        else printf("%s", buffer);
    }
    //wyrzucanie nieuzywanych kleintow
    if(revents & ~POLLIN){
        printf("removing %d\n", clientFd);
        descr.erase(descr.begin()+indexInDescr);
        
        shutdown(clientFd, SHUT_RDWR);
        close(clientFd);
    }
}



int main(int argc, char ** argv) {
    
    long port = 12346;
    
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
    //zapisujemy fd serwera do tablicy polla
    pollfd nacoczekac;
    nacoczekac.fd=fd;
    nacoczekac.events=POLLIN|POLLRDHUP;
    descr.push_back(nacoczekac);
    //main loop
    while(true){
        int ready = poll(descr.data(), descr.size(), -1);
        if(ready == -1){
            std::cout << "poll error"<<std::endl;
            return -1;
        }
        for(long unsigned int i = 0 ; i < descr.size() && ready > 0 ; ++i){
            if(descr[i].revents){
                if(descr[i].fd == fd)
                    server(descr[i].revents, fd);
                else
                    client(i);
                ready--;
            }
        }
    }
}

