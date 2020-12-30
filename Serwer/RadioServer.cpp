#include "Server.h"
/*
narazie serwer tylko przyjmuje wiadomosci i je wypisuje, ale robi to juz w miare wspolbieznie, wiec gicior, to jest poczatek do dostawania requestow od klienta
*/




int main(int argc, char ** argv) {
    Server* serv = new Server(std::stol(argv[1]));
    serv->start();
    
}

