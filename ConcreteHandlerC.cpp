#include "ConcreteHandlerC.h"
#include "Task.h"
#include "Command.h"
#include "LogCommand.h"
#include "TaskIterator.h"
#include <iostream>

ConcreteHandlerC::ConcreteHandlerC(Command* c) : Handler(c) {
    minLeaf = 7;
    maxLeaf = 9;
}

void ConcreteHandlerC::process(Task* t, int number) {
    if (t == nullptr || command == nullptr) return;
    TaskIterator* it = t->createIterator(number);
    for (it->first(); !it->isDone(); it->next()) {
        Task* currentTask = it->currentItem();
        if (currentTask == nullptr) continue;
        command->execute(currentTask);
        std::cout << "Handler C (category " << number << "): Subtask "<< currentTask->getName() << " done" << std::endl;
        if (logcommand != nullptr && it->allCompleted()) {
            logcommand->execute(t);   // very last task, log once
        }
    }
    delete it;
}