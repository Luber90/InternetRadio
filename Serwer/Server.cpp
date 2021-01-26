#include "Server.h"

Server::Server(long p){
    errno = 0;
    std::ifstream r;
    r.open("rooms.txt", std::ifstream::in);
    std::string data1, data2;
    int licz = 0, liczmuz = 0;
    while(true){
        liczmuz = 0;
        r >> data1;
        if(data1 == "!"){
            break;
        }
        r >> data2;
        Queue *tmp = new Queue(data2);
        rooms.push_back(new RoomInfo(data1, tmp));
        liczmuz++;
        while(true){
            r>>data2;
            if(data2==";"){
                tmp->setNext(rooms[licz]->getCurr());
                break;
            }
            tmp->setNext(new Queue(data2));
            tmp = tmp->getNext();
            liczmuz++;
        }
        rooms[licz]->setCount(liczmuz);
        licz++;
    }
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
void Server::sndMusic(){
    using namespace std::chrono;
    while(true){
        for(long unsigned int j = 0; j < rooms.size(); j++){
            delMtx.lock();
            if(rooms[j]->getUsrNumber()>0){
                //jesli nastepna probka juz miala byc
                if(rooms[j]->getNextSample()<=time_point_cast<nanoseconds>(system_clock::now())){
                    short bytes[256];
                    int ile = fread(bytes,sizeof(short),sizeof(bytes), rooms[j]->getMusic());
                    //duration<float> sampleDuration {ile/44100.0};
                    if(ile == 0){
                        if(!rooms[j]->getRandom()){
                            rooms[j]->nxtMusic();
                            ile = fread(bytes,sizeof(short),sizeof(bytes), rooms[j]->getMusic());
                        }
                        else{
                            rooms[j]->RndMusic();
                            ile = fread(bytes,sizeof(short),sizeof(bytes), rooms[j]->getMusic());
                        }
                        
                    }
                    //ustawiamy czas ostatniej probki i nastepnej probki na odpowiednie
                    rooms[j]->setLastSample(time_point_cast<nanoseconds>(system_clock::now()));
                    rooms[j]->setNextSample(time_point_cast<nanoseconds>(system_clock::now()) + nanoseconds(ile*22676));
                    for(long unsigned k = 0; k < rooms[j]->getUsrNumber(); k++){
                        if(!rooms[j]->getClients()[k]->getMute()){
                            sendto(udpfd, bytes, ile*sizeof(short), 0, (sockaddr*)rooms[j]->getClients()[k]->getAddr(), rooms[j]->getClients()[k]->getSize());
                        }
                    }
                }
            }
            delMtx.unlock();
        }
    }
}
void Server::start(){
    std::thread t1(&Server::sndMusic, this);
    while(true){
        int ready = poll(descr.data(), descr.size(), 0);
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
        //this->sndMusic();
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
        write(clientFd, ro.c_str(), ro.length());
        printf("%d %s\n", clientFd, ro.c_str());
    }
}

void Server::pollClient(int index){
    auto clientFd = descr[index].fd;
    auto revents = descr[index].revents;
    //pobieranie wiadomosci i wyswietlenie jej
    if(revents & POLLIN){
        char buffer[128] = "";
        int count = read(clientFd, buffer, 128);
        if(count < 1){
            revents |= POLLERR;
        }
        else {
            printf("%d: %s\n", clientFd, buffer);
            
            std::string buff = std::string(buffer);
            int cmd = this->getCmd(buff);
            std::string msg;
            switch(cmd){
            case czero:
                msg = "bad:";

                write(clientFd, msg.c_str(), msg.length());
                break;
            
            case croom:
                for(long unsigned int i = 0; i < rooms.size(); i++){
                    if(buff.substr(5)==rooms[i]->getName()){
                        for (long unsigned int j = 0; j < clients.size(); j ++){
                            if(clients[j]->getFd() == clientFd){
                                rooms[i]->addClient(clients[j]);
                                msg = "queue:"+rooms[i]->getFirst()->getName();
                                Queue *tmp = rooms[i]->getFirst()->ggetNext();
                                while(true){
                                    if(tmp == rooms[i]->getFirst()){
                                        break;
                                    }
                                    msg += ","+tmp->getName();
                                    tmp = tmp->ggetNext();
                                }
                                std::cout<<clientFd<<" "<<msg<<std::endl;
                                write(clientFd, msg.c_str(), 128);
                                tmp = rooms[i]->getFirst()->ggetNext();
                                while(true){
                                    if(tmp == rooms[i]->getFirst()){
                                        break;
                                    }
                                    if(!tmp->getActive()){
                                        std::string ro = "hide:"+tmp->getName()+";"+ (tmp->getActive() ? "yes" : "no");
                                        write(clientFd, ro.c_str(), ro.length());
                                    }
                                    if(tmp->getLoop()){
                                        std::string ro = "loop:"+tmp->getName()+";"+ (tmp->getLoop() ? "yes" : "no");
                                        write(clientFd, ro.c_str(), ro.length());
                                    }
                                    tmp = tmp->ggetNext();
                                }
                                break;
                            }
                            
                        }
                    }
                    else{
                        if(rooms[i]->getUsrNumber()>0){
                            for(long unsigned int j = 0; j < rooms[i]->getUsrNumber(); j++){
                                if(rooms[i]->getClients()[j]->getFd()==clientFd){
                                    delMtx.lock();
                                    rooms[i]->erase(j);
                                    delMtx.unlock();
                                }
                            }
                        }
                    }
                }
                break;
            case cup:
                for(long unsigned int i = 0; i < rooms.size(); i++){
                    if(buff.substr(3, buff.find(";")-3)!=rooms[i]->getName()){
                        continue;
                    }
                    Queue* prev = rooms[i]->getCurr();
                    Queue* tmpp = rooms[i]->getCurr()->ggetNext();
                    while(tmpp->getName()!=buff.substr(buff.find(";")+1)){
                        prev = tmpp;
                        tmpp = tmpp->ggetNext();
                    }
                    prev->exchange(tmpp);
                    msg = "queue:"+rooms[i]->getFirst()->getName();
                    Queue *tmp = rooms[i]->getFirst()->ggetNext();
                    while(true){
                        if(tmp == rooms[i]->getFirst()){
                            break;
                        }
                        msg += ","+tmp->getName();
                        tmp = tmp->ggetNext();
                    }
                    std::cout<<clientFd<<" "<<msg<<std::endl;
                    write(clientFd, msg.c_str(), 128);
                    tmp = rooms[i]->getFirst()->ggetNext();
                    while(true){
                        if(tmp == rooms[i]->getFirst()){
                            break;
                        }
                        if(!tmp->getActive()){
                            std::string ro = "hide:"+tmp->getName()+";"+ (tmp->getActive() ? "yes" : "no");
                            write(clientFd, ro.c_str(), ro.length());
                        }
                        if(tmp->getLoop()){
                            std::string ro = "loop:"+tmp->getName()+";"+ (tmp->getLoop() ? "yes" : "no");
                            write(clientFd, ro.c_str(), ro.length());
                        }
                        tmp = tmp->ggetNext();
                    }
                }
                break;
            case cdown:
                for(long unsigned int i = 0; i < rooms.size(); i++){
                    if(buff.substr(5, buff.find(";")-5)!=rooms[i]->getName()){
                        continue;
                    }
                    Queue* tmpp = rooms[i]->getCurr();
                    while(tmpp->getName()!=buff.substr(buff.find(";")+1)){
                        tmpp=tmpp->ggetNext();
                    }
                    tmpp->exchange(tmpp->ggetNext());
                    msg = "queue:"+rooms[i]->getFirst()->getName();
                    Queue *tmp = rooms[i]->getFirst()->ggetNext();
                    while(true){
                        if(tmp == rooms[i]->getFirst()){
                            break;
                        }
                        msg += ","+tmp->getName();
                        tmp = tmp->ggetNext();
                    }
                    std::cout<<clientFd<<" "<<msg<<std::endl;
                    write(clientFd, msg.c_str(), 128);
                    tmp = rooms[i]->getFirst()->ggetNext();
                    while(true){
                        if(tmp == rooms[i]->getFirst()){
                            break;
                        }
                        if(!tmp->getActive()){
                            std::string ro = "hide:"+tmp->getName()+";"+ (tmp->getActive() ? "yes" : "no");
                            write(clientFd, ro.c_str(), ro.length());
                        }
                        if(tmp->getLoop()){
                            std::string ro = "loop:"+tmp->getName()+";"+ (tmp->getLoop() ? "yes" : "no");
                            write(clientFd, ro.c_str(), ro.length());
                        }
                        tmp = tmp->ggetNext();
                    }
                }
                
                break;
            case cmute:
                for(long unsigned int i = 0; i < clients.size(); i++){
                    if(clientFd == clients[i]->getFd()){
                        clients[i]->chngMute();
                        break;
                    }
                }
                break;
            case chide:
                for(long unsigned int i = 0; i < rooms.size(); i++){
                    if(buff.substr(5, buff.find(";")-5)!=rooms[i]->getName()){
                        continue;
                    }
                    Queue* tmpp = rooms[i]->getCurr();
                    while(tmpp->getName()!=buff.substr(buff.find(";")+1)){
                        tmpp=tmpp->ggetNext();
                    }
                    tmpp->setActive(!tmpp->getActive());
                    for(long unsigned int j = 0; j< rooms[i]->getClients().size(); j++){
                        std::string ro = "hide:"+tmpp->getName()+";"+ (tmpp->getActive() ? "yes" : "no");
                        write(clientFd, ro.c_str(), ro.length());
                    }
                }
                break;
            case cloop:
                for(long unsigned int i = 0; i < rooms.size(); i++){
                    if(buff.substr(5, buff.find(";")-5)!=rooms[i]->getName()){
                        continue;
                    }
                    Queue* tmpp = rooms[i]->getCurr();
                    while(tmpp->getName()!=buff.substr(buff.find(";")+1)){
                        tmpp=tmpp->ggetNext();
                    }
                    tmpp->setLoop(!tmpp->getLoop());
                    for(long unsigned int j = 0; j< rooms[i]->getClients().size(); j++){
                        std::string ro = "loop:"+tmpp->getName()+";"+ (tmpp->getLoop() ? "yes" : "no");
                        write(clientFd, ro.c_str(), ro.length());
                    }
                }
                break;
            case crandom:
                for(long unsigned int i = 0; i < rooms.size(); i++){
                    if(buff.substr(7, buff.find(";")-7)!=rooms[i]->getName()){
                        continue;
                    }
                    rooms[i]->chngRandom();
                }
                break;
                
            }
            
        }
    }
    //wyrzucanie nieuzywanych kleintow
    if(revents & ~POLLIN){
        printf("removing %d\n", clientFd);
        long unsigned int i1, i2, i3;
        for (long unsigned int i=0; i < clients.size(); i++){
            if(clients[i]->getFd()==clientFd){
                i1 = i;
                for (long unsigned int j = 0; j < rooms.size(); j++){
                    for(long unsigned int y = 0; y < rooms[j]->getUsrNumber(); y++){
                        if(rooms[j]->getClients()[y]->getFd()==clientFd){
                            i2 = j;
                            i3 = y;
                            break;
                        }
                    }
                }
                break;
            }
        }
        delMtx.lock();
        clients.erase(clients.begin()+i1);
        rooms[i2]->erase(i3);
        descr.erase(descr.begin()+index);
        delMtx.unlock();
        
        shutdown(clientFd, SHUT_RDWR);
        close(clientFd);
    }
}

void Server::sendMusic(sockaddr_in* ad, std::string name){
    //NIC
}

Cmd Server::getCmd(std::string cmd){
    int index = cmd.find(":");
    if (std::string(&cmd[0], &cmd[index])=="room"){
        return croom;
    }
    else if(std::string(&cmd[0], &cmd[index])=="up"){
        return cup;
    }
    else if(std::string(&cmd[0], &cmd[index])=="down"){
        return cdown;
    }
    else if(std::string(&cmd[0], &cmd[index])=="mute"){
        return cmute;
    }
    else if(std::string(&cmd[0], &cmd[index])=="hide"){
        return chide;
    }
    else if(std::string(&cmd[0], &cmd[index])=="loop"){
        return cloop;
    }
    else if(std::string(&cmd[0], &cmd[index])=="random"){
        return crandom;
    }
    else return czero;
}