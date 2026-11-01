#ifndef CONCRETEHANDLERA_H
#define CONCRETEHANDLERA_H

#include "Handler.h"

//Handles categories 1 to 3
class ConcreteHandlerA : public Handler{
protected:
    void process(Task* t, int number) override;
public:
    explicit ConcreteHandlerA(Command* c = nullptr);
};
#endif