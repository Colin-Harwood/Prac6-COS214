#include "ConcreteHandlerA.h"
#include "Task.h"
#include "Command.h"
#include "TaskIterator.h"
#include <iostream>
ConcreteHandlerA::ConcreteHandlerA(Command* c):Handler(c){
    minLeaf = 1;
    maxLeaf = 3;
}

//  Connected ConcreteHandler to TaskIterator - Motheo
void ConcreteHandlerA::process(Task* t, int number) {
    if (t == nullptr) return;

    TaskIterator* it = t->createIterator(number);
    for(it->first(); !it->isDone(); it->next())
    {
        Task* currentTask = it->currentItem();
        if(currentTask != nullptr && command != nullptr)
        {
            currentTask->execute();
            command->execute(currentTask);
            std::cout << "Handler A (category " << number << "): Subtask "<<currentTask->getName()<< " done" << std::endl;
        }
    }
    delete it;
}