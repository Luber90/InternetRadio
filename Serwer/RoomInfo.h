#include <vector>
#include <string>

class RoomInfo{
public:
    RoomInfo(std::string n);
    void addFd(int fd);
    std::vector<int> getFds();
    int getUsrNumber();
    std::string getName();
private:
    std::string name;
    std::vector<int> fds;
};
