#include "ConcreteHandlerA.h"
#include "Task.h"
#include "Command.h"
#include "LogCommand.h"
#include "TaskIterator.h"
#include "TimeoutMonitor.h"
#include <iostream>
/**
 * @file ConcreteHandlerA.cpp
 * @brief Chain of Responsibility handler for categories 1-3.
 *
 * Iterates over matching subtasks, attaches a TimeoutMonitor to each,
 * runs the command and logs once all subtasks are completed.
 */

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

        TimeoutMonitor* monitor = new TimeoutMonitor(currentTask);
        currentTask->addObserver(monitor);

        bool done = runTask(currentTask, "Handler A");

        currentTask->removeObserver(monitor);   // detaches and deletes the monitor

        if (done) {
            std::cout << "Handler A (category " << number << "): Subtask "
                      << currentTask->getName() << " done" << std::endl;
        }

        if (logcommand != nullptr && it->allCompleted()) {
            logcommand->execute(t);
        }
    }
    delete it;
}