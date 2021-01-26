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
#include "RoomInfo.h"
#include <fstream>
#include <mutex>
#include <algorithm>

enum Cmd {czero, croom, cup, cdown, cmute, cloop, crandom, chide};

//jeszcze ni uzywane, ale bedzie potrzebne w obiektowosci

void sndMusic(FILE *music, std::vector<KlientInfo*> clients, std::vector<RoomInfo*> rooms, int udpfd);

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
    void sndMusic();
    void sendMusic(sockaddr_in* ad, std::string name);
    Cmd getCmd(std::string cmd);
    std::mutex delMtx;
public:
    Server(long p);
    void start();
    
};