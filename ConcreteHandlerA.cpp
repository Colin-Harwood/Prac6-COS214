#include "ConcreteHandlerA.h"
#include "Task.h"
#include <iostream>
ConcreteHandlerA::ConcreteHandlerA(Command* c):Handler(c){
    minLeaf = 1;
    maxLeaf = 3;
}

void ConcreteHandlerA::process(Task* t, int number) {
    std::cout << "Handler A (category " << number << "): " + "Subtask done";
    t->execute();
}