// ==================================================================
//  COPY 1 of 2 - UML VERSION (matches the class diagram exactly)
// ==================================================================
#ifndef TIMEDOUTSTATE_H
#define TIMEDOUTSTATE_H

#include "TaskState.h"

/**
 * TimedOutState = ConcreteState (State pattern)
 * Missed its deadline; must be picked up again (escalated).
 * Allowed: start -> InProgress, cancel -> Cancelled
 */
class TimedOutState : public TaskState {
public:
    TaskState* start(Task* t) override;
    TaskState* cancel(Task* t) override;
    std::string getName() const override;
    TaskState* clone() const override;
};

#endif
