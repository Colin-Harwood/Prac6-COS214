#ifndef CONCRETEHANDLERB_H
#define CONCRETEHANDLERB_H

#include "Handler.h"

//Handles categories 4to6
class ConcreteHandlerB : public Handler {
protected:
    void process(Task* t, int number) override;
public:
    explicit ConcreteHandlerB(Command* c = nullptr);
};
#endif