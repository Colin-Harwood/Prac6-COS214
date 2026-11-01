/**
 * @file InProgressState.h
 * @brief Concrete state: a task that is being worked on.
 *
 * Role in the State pattern: ConcreteState.
 */
#ifndef INPROGRESSSTATE_H
#define INPROGRESSSTATE_H

#include "TaskState.h"

/**
 * @brief A task that is being worked on.
 */
class InProgressState : public TaskState {
public:
    /**
     * @brief Completes the task.
     *
     * @param t Task the action applies to (not used).
     * @return A new CompletedState.
     */
    TaskState* complete(Task* t) override;
    /**
     * @brief Rejects the task.
     *
     * @param t Task the action applies to (not used).
     * @return A new RejectedState.
     */
    TaskState* reject(Task* t) override;
    /**
     * @brief Marks the task as timed out.
     *
     * @param t Task the action applies to (not used).
     * @return A new TimedOutState.
     */
    TaskState* timeout(Task* t) override;
    /**
     * @brief Cancels the task.
     *
     * @param t Task the action applies to (not used).
     * @return A new CancelledState.
     */
    TaskState* cancel(Task* t) override;
    /**
     * @brief Gets the name of this state.
     *
     * @return "InProgress".
     */
    std::string getName() const override;
    /**
     * @brief Copies this state.
     *
     * @return A new InProgressState, owned by the caller.
     */
    TaskState* clone() const override;
};

#endif
