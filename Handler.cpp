#include "Handler.h"
#include "Task.h"
#include "Command.h"
#include "LogCommand.h"
#include "TaskIterator.h"
#include "TaskState.h"      // InvalidTransition
#include <iostream>


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
    // Pending/TimedOut -> InProgress
    try {
        task->start();
    } catch (const InvalidTransition&) {
        // can't start: only carry on if it is already InProgress
        if (task->getStateName() != "InProgress") {
            std::cout << label << ": skipped " << task->getName()
                      << " (" << task->getStateName() << ")" << std::endl;
            return false;
        }
    }

    command->execute(task);

    // InProgress -> Completed
    try {
        task->complete();
    } catch (const InvalidTransition&) {
        // monitor timed it out mid-work: TimedOut -> InProgress -> Completed
        try {
            task->start();
            task->complete();
        } catch (const InvalidTransition& e) {
            std::cout << label << ": " << e.what() << std::endl;
            return false;
        }
    }
    return true;
}