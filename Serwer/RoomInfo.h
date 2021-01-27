#include <vector>
#include <string>
#include "KlientInfo.h"
#include "Queue.h"
#include <chrono>


//przechowuje informacje o pokoju, klientach w pokoju, czasie i zarzadza piosenkami
class RoomInfo{
public:
    RoomInfo(std::string n, Queue* c);
    void addClient(KlientInfo * c);
    std::vector<KlientInfo*> getClients();
    long unsigned int getUsrNumber();
    std::string getName();
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> getLastSample();
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> getNextSample();
    void setLastSample(std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> t);
    void setNextSample(std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> t);
    Queue* getCurr();
    Queue* getFirst();
    FILE* getMusic();
    void nxtMusic();
    void erase(int i);
    void setCount(int a);
    int getCount();
    void RndMusic();
    void chngRandom();
    bool getRandom();
private:
    bool random;
    Queue *first;
    Queue *curr;
    FILE *music;
    std::string name;
    int musicCount;
    std::vector<KlientInfo*> clients;
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> lastSample, nextSample;;

};
