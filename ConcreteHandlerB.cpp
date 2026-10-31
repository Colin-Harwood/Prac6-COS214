#include "ConcreteHandlerB.h"
#include "Task.h"
#include "Command.h"
#include "TaskIterator.h"
#include <iostream>

ConcreteHandlerB::ConcreteHandlerB(Command* c) : Handler(c) {
    minLeaf = 4;
    maxLeaf = 6;
}

void ConcreteHandlerB::process(Task* t, int number) {
    if (t == nullptr || command == nullptr) return;

    TaskIterator* it = t->createIterator(number);
    for (it->first(); !it->isDone(); it->next()) {
        Task* currentTask = it->currentItem();
        if (currentTask == nullptr) continue;

        command->execute(currentTask);   // start() + complete() happen inside the command
        std::cout << "Handler B (category " << number << "): Subtask "
                  << currentTask->getName() << " done" << std::endl;
    }
    delete it;
}