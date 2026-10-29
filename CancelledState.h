// ==================================================================
//  COPY 1 of 2 - UML VERSION (matches the class diagram exactly)
// ==================================================================
#ifndef CANCELLEDSTATE_H
#define CANCELLEDSTATE_H

#include "TaskState.h"

/**
 * CancelledState = ConcreteState (State pattern)
 * Withdrawn. Final.
 * Allowed: nothing (final state)
 */
class CancelledState : public TaskState {
public:
    std::string getName() const override;
    bool isFinished() const override;
    TaskState* clone() const override;
};

#endif
