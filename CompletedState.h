// ==================================================================
//  COPY 1 of 2 - UML VERSION (matches the class diagram exactly)
// ==================================================================
#ifndef COMPLETEDSTATE_H
#define COMPLETEDSTATE_H

#include "TaskState.h"

/**
 * CompletedState = ConcreteState (State pattern)
 * Done and accepted. Only rework can reopen it.
 * Allowed: reopen -> Pending
 */
class CompletedState : public TaskState {
public:
    TaskState* reopen(Task* t) override;
    std::string getName() const override;
    bool isFinished() const override;
    TaskState* clone() const override;
};

#endif
