// ==================================================================
//  COPY 1 of 2 - UML VERSION (matches the class diagram exactly)
// ==================================================================
#ifndef INPROGRESSSTATE_H
#define INPROGRESSSTATE_H

#include "TaskState.h"

/**
 * InProgressState = ConcreteState (State pattern)
 * A handler/person is responsible for it right now.
 * Allowed: complete -> Completed, reject -> Rejected, timeout -> TimedOut, cancel -> Cancelled
 */
class InProgressState : public TaskState {
public:
    TaskState* complete(Task* t) override;
    TaskState* reject(Task* t) override;
    TaskState* timeout(Task* t) override;
    TaskState* cancel(Task* t) override;
    std::string getName() const override;
    TaskState* clone() const override;
};

#endif
