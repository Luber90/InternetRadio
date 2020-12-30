#include "Server.h"

Server::Server(long p){
    /*music = new std::ifstream("sad.wav", std::ios::binary);
    if (*music){
        printf("blad\n");
    }*/
    errno = 0;
    music = fopen("Serwer/sad.wav", "rb");
    if(music == NULL){
        printf("nie\n");
    }
    printf("Error %d \n", errno);
    rooms.push_back(new RoomInfo("gierki"));
    rooms.push_back(new RoomInfo("shitpost"));
    rooms.push_back(new RoomInfo("general"));
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
        int ready = poll(descr.data(), descr.size(), 1);
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
        for(long unsigned int i = 0; i < clients.size(); i++){
            for(long unsigned int j = 0; j < rooms.size(); j++){
                for(long unsigned int k = 0; k < rooms[j]->getFds().size(); k++){
                    if(rooms[j]->getFds()[k]==clients[i]->getFd()){
                        this->sendMusic(clients[i]->getAddr(), rooms[j]->getName());
                        short bytes[8128];
                        k = fread(bytes,sizeof(short),2, music);
                        sendto(udpfd, bytes, k*sizeof(short), 0, (sockaddr*)clients[i]->getAddr(), clients[i]->getSize());
                        sleep(0.35);
                    }
                }
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
        clients.push_back(new KlientInfo(clientFd, clientAddr));
        //zwraca do klienta pokoje, to jest mega prowizorka i bedzie zmienione
        std::string ro = "rooms:";
        for (long unsigned int i = 0; i < rooms.size(); i++){
            ro = ro + rooms[i]->getName();
            if(i != rooms.size()-1){
                ro = ro+",";
            }
        }
        write(clientFd, ro.c_str(), 255);
        printf("%d %s\n", clientFd, ro.c_str());
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
            printf("%d: %s\n", clientFd, buffer);
            
            std::string buff = std::string(buffer);
            int cmd = this->getCmd(buff);
            std::string msg;
            std::cout<<"mordoooo"<<std::endl;
            switch(cmd){
            case 0:
                msg = "bad:";

                write(clientFd, msg.c_str(), 255);
                break;
            
            case 1:
                for(long unsigned int i = 0; i < rooms.size(); i++){
                    if(std::string(&buff[5], &buff[buff.size()])==rooms[i]->getName()){
                        rooms[i]->addFd(clientFd);
                    }
                }
                msg = "queue:madonna,sonic";
                write(clientFd, msg.c_str(), 255);
            }
            
        }
    }
    //wyrzucanie nieuzywanych kleintow
    if(revents & ~POLLIN){
        printf("removing %d\n", clientFd);
        for (long unsigned int i=0; i < clients.size(); i++){
            if(clients[i]->getFd()==clientFd){
                clients.erase(clients.begin()+i);
            }
        }
        descr.erase(descr.begin()+index);
        
        shutdown(clientFd, SHUT_RDWR);
        close(clientFd);
    }
}

void Server::sendMusic(sockaddr_in* ad, std::string name){
    //NIC
}

int Server::getCmd(std::string cmd){
    int index = cmd.find(":");
    if (std::string(&cmd[0], &cmd[index])=="room"){
        return 1;
    }
    else return 0;
}