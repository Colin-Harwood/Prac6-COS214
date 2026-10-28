#include "ConcreteHandlerC.h"
#include "Task.h"
#include <iostream>
ConcreteHandlerC::ConcreteHandlerC(Command* c) : Handler(c) {
    minLeaf = 7;
    maxLeaf = 9;
}
void ConcreteHandlerC::process(Task* t, int number) {
    std::cout << "Handler C (category " << number << "): Subtask done" << std::endl;
    t->execute();
}