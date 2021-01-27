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
#include <climits>

enum Cmd {czero, croom, cup, cdown, cmute, cloop, crandom, chide, cskip};

void sndMusic(FILE *music, std::vector<KlientInfo*> clients, std::vector<RoomInfo*> rooms, int udpfd);

struct mess{
    std::string msg;
    int fd;
    int wyslano;
};


class Server{
private:
    long port;
    std::vector<pollfd> descr;
    std::vector<mess> m;
    sockaddr_in myAddr {};
    int tcpfd, udpfd;
    std::vector<KlientInfo*> clients;
    std::vector<RoomInfo*> rooms;
    void pollServer(int revents);
    void pollClient(int index);
    void sndMusic();
    Cmd getCmd(std::string cmd);
    void sndMess();
    std::mutex delMtx;
    std::mutex msgMtx;
public:
    Server(long p);
    void start();
    
};