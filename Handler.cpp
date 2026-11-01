#include "Handler.h"
#include "Task.h"
#include "Command.h"
#include "LogCommand.h"
#include "TaskIterator.h"
#include <iostream>
#include <exception>


Handler::Handler(Command* c): next(nullptr), minLeaf(0), maxLeaf(0), command(c), logcommand(nullptr) {}
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
void Handler::setLogCommand(LogCommand* l){logcommand = l;}

bool Handler::runTask(Task* task, const char* label){
    try {
        command->execute(task);
    } catch (const std::exception& e) {
        std::cout << label << ": " << e.what() << std::endl;
        return false;
    }
    return true;
}