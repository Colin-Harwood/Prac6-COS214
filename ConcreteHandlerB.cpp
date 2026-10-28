#include "ConcreteHandlerB.h"
#include "Task.h"
#include <iostream>
ConcreteHandlerB::ConcreteHandlerB(Command* c) : Handler(c) {
    minLeaf = 4;
    maxLeaf = 6;
}
void ConcreteHandlerB::process(Task* t, int number) {
    std::cout << "Handler B (category " << number << "): Subtask done" << std::endl;
    t->execute();
}