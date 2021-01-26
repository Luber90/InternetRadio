#include "Queue.h"

Queue::Queue(std::string n){
    name = n;
    loop = false;
    next = nullptr;
    active = true;
}

std::string Queue::getName(){
    return name;
}

void Queue::setLoop(bool l){
    loop = l;
}

void Queue::setNext(Queue* n){
    next = n;
}

bool Queue::getActive(){
    return active;
}

Queue* Queue::getNext(){
    if(loop&&active){
        return this; 
    }
    else if(!next->getActive()){
        return next->getNext();
    }
    else{
        return next;
    }

}

Queue* Queue::ggetNext(){
    return next;
}

bool Queue::getLoop(){
    return loop;
}

void Queue::setName(std::string s){
    name = s;
}

void Queue::setActive(bool a){
    active = a;
}

void Queue::exchange(Queue* exchanger){
    std::string tmp = this->name;
    bool tmploop = this->loop, tmpactive = this->active;
    name = exchanger->getName();
    loop = exchanger->getLoop();
    active = exchanger->getActive();
    exchanger->setName(tmp);
    exchanger->setLoop(tmploop);
    exchanger->setActive(tmpactive);
}