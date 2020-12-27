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

//jeszcze ni uzywane, ale bedzie potrzebne w obiektowosci

class Server{
private:
    long port;
    std::vector<pollfd> descr;
    char rooms[255] = "gierki, shitpost, general";
    sockaddr_in myAddr {};
    int tcpfd, udpfd;
    void pollServer(int revents);
    void pollClient(int index);
public:
    Server(long p);
    void start();
    
};