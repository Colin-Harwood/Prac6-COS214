#include "ConcreteHandlerC.h"
#include "Task.h"
#include "Command.h"
#include "LogCommand.h"
#include "TaskIterator.h"
#include "TimeoutMonitor.h"
#include <iostream>
/**
 * @file ConcreteHandlerC.cpp
 * @brief Chain of Responsibility handler for categories 7-9.
 *
 * Iterates over matching subtasks, attaches a TimeoutMonitor to each,
 * runs the command and logs once all subtasks are completed.
 */
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

        TimeoutMonitor* monitor = new TimeoutMonitor(currentTask);
        currentTask->addObserver(monitor);

        bool done = runTask(currentTask, "Handler C");

        currentTask->removeObserver(monitor);

        if (done) {
            std::cout << "Handler C (category " << number << "): Subtask "
                      << currentTask->getName() << " done" << std::endl;
        }

        if (logcommand != nullptr && it->allCompleted()) {
            logcommand->execute(t);
        }
    }
    delete it;
}