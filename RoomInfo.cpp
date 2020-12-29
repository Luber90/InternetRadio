#include "RoomInfo.h"

RoomInfo::RoomInfo(std::string n){
    name = n;
}
void RoomInfo::addFd(int fd){
    fds.push_back(fd);
}
std::vector<int> RoomInfo::getFds(){
    return fds;
}
int RoomInfo::getUsrNumber(){
    return fds.size();
}
std::string RoomInfo::getName(){
    return name;
}