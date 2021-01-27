#include <string>

//jest do "czesc" listy jednokierunkowej z piosenkami

class Queue{
public:
    Queue(std::string n);
    void setNext(Queue* n);
    Queue* getNext();
    Queue* ggetNext();
    void setLoop(bool l);
    bool getLoop();
    bool getActive();
    std::string getName();
    void exchange(Queue* exchanger);
    void setName(std::string s);
    void setActive(bool a);
private:
    std::string name;
    Queue* next;
    bool loop, active;
};