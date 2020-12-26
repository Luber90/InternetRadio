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
#include<vector>

//jeszcze ni uzywane, ale bedzie potrzebne w obiektowosci

class Server{
private:
    std::vector<int> fds;
    
};