#include "Handler.h"
#include "Task.h"
#include <iostream>

Handler::Handler(Command* c):next(nullptr),minLeaf(0),maxLeaf(0),command(c){}
Handler::~Handler(){}
bool Handler::canHandle(int number) const{
    return number >= minLeaf && number <= maxLeaf;
}
void Handler::setNext(Handler* h){next = h;}
void Handler::handle(Task* t, int number){
    if (canHandle(number)){
        process(t, number);
    } else if (next){
        next->handle(t, number);
    } else{
        std::cout << "No handler for category ERROR (task category wrong)" << number << std::endl;
    }
}