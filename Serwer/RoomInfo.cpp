#include "RoomInfo.h"

RoomInfo::RoomInfo(std::string n, Queue *c){
    using namespace std::chrono;
    first = c;
    curr = c;
    music = fopen(curr->getName().c_str(), "rb");
    name = n;
    random = false;
    lastSample = time_point_cast<time_point<system_clock, nanoseconds>::duration>(system_clock::time_point(system_clock::now()));
    nextSample = time_point_cast<time_point<system_clock, nanoseconds>::duration>(system_clock::time_point(system_clock::now()));
}
/*
void RoomInfo::addFd(int fd){
    fds.push_back(fd);
}
std::vector<int> RoomInfo::getFds(){
    return fds;
}
*/
void RoomInfo::addClient(KlientInfo * c){
    clients.push_back(c);
}
std::vector<KlientInfo*> RoomInfo::getClients(){
    return clients;
}
long unsigned int RoomInfo::getUsrNumber(){
    return clients.size();
}
std::string RoomInfo::getName(){
    return name;
}
std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> RoomInfo::getLastSample(){
    return lastSample;
}
std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> RoomInfo::getNextSample(){
    return nextSample;
}
void RoomInfo::setLastSample(std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> t){
    lastSample = t;
}
void RoomInfo::setNextSample(std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> t){
    nextSample = t;
}
Queue* RoomInfo::getCurr(){
    return curr;
}
Queue* RoomInfo::getFirst(){
    return first;
}
FILE* RoomInfo::getMusic(){
    return music;
}
void RoomInfo::nxtMusic(){
    fclose(music);
    curr = curr->getNext();
    music = fopen(curr->getName().c_str(), "rb");
}
void RoomInfo::erase(int i){
    clients.erase(clients.begin()+i);
}
void RoomInfo::setCount(int a){
    musicCount = a;
}
int RoomInfo::getCount(){
    return musicCount;
}
void RoomInfo::RndMusic(){
    fclose(music);
    int ile = rand() % musicCount;
    Queue *tmp = curr;
    for(int i = 0; i < ile; i++){
        tmp = tmp->getNext();
    }
    curr = tmp;
    music = fopen(curr->getName().c_str(), "rb");
}
void RoomInfo::chngRandom(){
    random = !random;
}
bool RoomInfo::getRandom(){
    return random;
}