#include "Server.h"

//funkcja main


int main(int argc, char ** argv) {
    Server* serv = new Server(std::stol(argv[1]));
    serv->start();
    
}

