#include <sys/socket.h>
#include <arpa/inet.h>

//przechowuje informacje o kliencie

class KlientInfo{
private:
    int fd;
    sockaddr_in clientAddr{};
    socklen_t clientAddrSize = sizeof(clientAddr);
    bool mute;
public:
    KlientInfo(int f, sockaddr_in c);
    void chngMute();
    bool getMute();
    int getFd();
    sockaddr_in* getAddr();
    socklen_t getSize();
};