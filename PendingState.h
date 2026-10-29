// ==================================================================
//  COPY 1 of 2 - UML VERSION (matches the class diagram exactly)
// ==================================================================
#ifndef PENDINGSTATE_H
#define PENDINGSTATE_H

#include "TaskState.h"

/**
 * PendingState = ConcreteState (State pattern)
 * Created, nobody is working on it yet.
 * Allowed: start -> InProgress, cancel -> Cancelled
 */
class PendingState : public TaskState {
public:
    TaskState* start(Task* t) override;
    TaskState* cancel(Task* t) override;
    std::string getName() const override;
    TaskState* clone() const override;
};

#endif
