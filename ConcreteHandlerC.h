/**
 * @file ConcreteHandlerC.h
 * @brief Handler for tasks with department categories 7 to 9.
 */

#ifndef CONCRETEHANDLERC_H
#define CONCRETEHANDLERC_H

#include "Handler.h"

/**
 * @brief Handles categories 7 to 9.
 */
class ConcreteHandlerC : public Handler {
protected:
    void process(Task* t, int number) override;
public:
    explicit ConcreteHandlerC(Command* c = nullptr);
};
#endif
