// ==================================================================
//  COPY 1 of 2 - UML VERSION (matches the class diagram exactly)
// ==================================================================
#ifndef REJECTEDSTATE_H
#define REJECTEDSTATE_H

#include "TaskState.h"

/**
 * RejectedState = ConcreteState (State pattern)
 * Done but refused; must be reworked.
 * Allowed: reopen -> Pending
 */
class RejectedState : public TaskState {
public:
    TaskState* reopen(Task* t) override;
    std::string getName() const override;
    TaskState* clone() const override;
};

#endif
