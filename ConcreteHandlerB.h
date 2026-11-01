/**
 * @file ConcreteHandlerB.h
 * @brief Handler for tasks with department categories 4 to 6.
 */

#ifndef CONCRETEHANDLERB_H
#define CONCRETEHANDLERB_H

#include "Handler.h"

/**
 * @brief Handles categories 4 to 6.
 */
class ConcreteHandlerB : public Handler {
protected:
    void process(Task* t, int number) override;
public:
    explicit ConcreteHandlerB(Command* c = nullptr);
};
#endif
