/**
 * @file RejectedState.h
 * @brief Concrete state: a task that was turned down and holds the workflow open until it is reopened.
 *
 * Role in the State pattern: ConcreteState.
 */
#ifndef REJECTEDSTATE_H
#define REJECTEDSTATE_H

#include "TaskState.h"

/**
 * @brief A task that was turned down and holds the workflow open until it is reopened.
 */
class RejectedState : public TaskState {
public:
    /**
     * @brief Reopens the task.
     *
     * @param t Task the action applies to (not used).
     * @return A new PendingState.
     */
    TaskState* reopen(Task* t) override;
    /**
     * @brief Gets the name of this state.
     *
     * @return "Rejected".
     */
    std::string getName() const override;
    /**
     * @brief Copies this state.
     *
     * @return A new RejectedState, owned by the caller.
     */
    TaskState* clone() const override;
};

#endif
