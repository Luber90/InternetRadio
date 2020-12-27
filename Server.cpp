#include "Server.h"

//jeszcze ni uzywane, ale bedzie potrzebne w obiektowosci

Server::Server(long p){
    port = p;
    myAddr.sin_family = AF_INET;
    myAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    myAddr.sin_port = htons((uint16_t)port);
    
    tcpfd = socket(AF_INET, SOCK_STREAM, 0);
    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
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
    
    fail = listen(tcpfd, 1);
    if(fail){
        perror("listen failed");
        exit(1);
    }
    pollfd tmp;
    tmp.fd=tcpfd;
    tmp.events=POLLIN|POLLRDHUP;
    descr.push_back(tmp);
}

void Server::start(){
    while(true){
        int ready = poll(descr.data(), descr.size(), -1);
            if(ready == -1){
                std::cout << "poll error"<<std::endl;
                exit(-1);
            }
            for(long unsigned int i = 0 ; i < descr.size() && ready > 0 ; ++i){
                if(descr[i].revents){
                    if(descr[i].fd == tcpfd)
                        this->pollServer(descr[i].revents);
                    else
                        this->pollClient(i);
                    ready--;
            }
        }
    }
}


void Server::pollServer(int revents){
    if(revents & ~POLLIN){
        std::cout << errno << "Event "<<revents<<"on server socket"<<std::endl;
        return;
    }
    //dodawanie nowego fd klienta do descr
    if(revents & POLLIN){
        sockaddr_in clientAddr{};
        socklen_t clientAddrSize = sizeof(clientAddr);
        
        auto clientFd = accept(tcpfd, (sockaddr*) &clientAddr, &clientAddrSize);
        if(clientFd == -1) std::cout<<"accept failed"<<std::endl;
        
        pollfd tmp;
        tmp.fd = clientFd;
        tmp.events = POLLIN|POLLRDHUP;
        descr.push_back(tmp);
        printf("new connection from: %s:%hu (fd: %d)\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), clientFd);
        //zwraca do klienta pokoje, to jest mega prowizorka i bedzie zmienione
        char text[255] = "test"; 
        sendto(udpfd, text, 255, 0, (sockaddr*) &clientAddr,  clientAddrSize);
        write(clientFd, rooms, 255);
    }
}

void Server::pollClient(int index){
    auto clientFd = descr[index].fd;
    auto revents = descr[index].revents;
    //pobieranie wiadomosci i wyswietlenie jej
    if(revents & POLLIN){
        char buffer[255] = "";
        int count = read(clientFd, buffer, 255);
        if(count < 1)
            revents |= POLLERR;
        else {
            printf("%s\n", buffer);
            
        }
    }
    //wyrzucanie nieuzywanych kleintow
    if(revents & ~POLLIN){
        printf("removing %d\n", clientFd);
        descr.erase(descr.begin()+index);
        
        shutdown(clientFd, SHUT_RDWR);
        close(clientFd);
    }
}