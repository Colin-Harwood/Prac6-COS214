#include "ConcreteHandlerB.h"
#include "Task.h"
#include "Command.h"
#include "LogCommand.h"
#include "TaskIterator.h"
#include "TimeoutMonitor.h"
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

        TimeoutMonitor* monitor = new TimeoutMonitor(currentTask);
        currentTask->addObserver(monitor);

        bool done = runTask(currentTask, "Handler B");

        currentTask->removeObserver(monitor);

        if (done) {
            std::cout << "Handler B (category " << number << "): Subtask "
                      << currentTask->getName() << " done" << std::endl;
        }

        if (logcommand != nullptr && it->allCompleted()) {
            logcommand->execute(t);
        }
    }
    delete it;
}