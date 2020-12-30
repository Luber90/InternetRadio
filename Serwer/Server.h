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
#include <string.h>
#include "KlientInfo.h"
#include "RoomInfo.h"

//jeszcze ni uzywane, ale bedzie potrzebne w obiektowosci

class Server{
private:
    long port;
    std::vector<pollfd> descr;
    char roomss[255] = "rooms:gierki,shitpost,general";
    sockaddr_in myAddr {};
    int tcpfd, udpfd;
    std::vector<KlientInfo*> clients;
    std::vector<RoomInfo*> rooms;
    void pollServer(int revents);
    void pollClient(int index);
    void sendMusic(sockaddr_in* ad, std::string name);
    int getCmd(std::string cmd);
public:
    Server(long p);
    void start();
    
};