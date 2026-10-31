#include "ConcreteHandlerA.h"
#include "Task.h"
#include "Command.h"
#include "LogCommand.h"
#include "TaskIterator.h"
#include <iostream>

ConcreteHandlerA::ConcreteHandlerA(Command* c) : Handler(c) {
    minLeaf = 1;
    maxLeaf = 3;
}

void ConcreteHandlerA::process(Task* t, int number) {
    if (t == nullptr || command == nullptr) return;

    TaskIterator* it = t->createIterator(number);
    for (it->first(); !it->isDone(); it->next()) {
        Task* currentTask = it->currentItem();
        if (currentTask == nullptr) continue;

        command->execute(currentTask);
        std::cout << "Handler A (category " << number << "): Subtask "<< currentTask->getName() << " done" << std::endl;

        if (logcommand != nullptr && it->allCompleted()) {
            logcommand->execute(t);   // very last task, log once
        }
    }
    delete it;
}