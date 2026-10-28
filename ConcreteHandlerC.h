#ifndef CONCRETEHANDLERC_H
#define CONCRETEHANDLERC_H

#include "Handler.h"

//Handles categories 7to9
class ConcreteHandlerC : public Handler {
protected:
    void process(Task* t, int number) override;
public:
    explicit ConcreteHandlerC(Command* c = nullptr);
};
#endif