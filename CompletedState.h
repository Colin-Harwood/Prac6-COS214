/**
 * @file CompletedState.h
 * @brief Concrete state: a task that finished successfully.
 *
 * Role in the State pattern: ConcreteState.
 */
#ifndef COMPLETEDSTATE_H
#define COMPLETEDSTATE_H

#include "TaskState.h"

/**
 * @brief A task that finished successfully.
 */
class CompletedState : public TaskState {
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
     * @return "Completed".
     */
    std::string getName() const override;
    /**
     * @brief Checks whether this state counts as finished.
     *
     * @return True.
     */
    bool isFinished() const override;
    /**
     * @brief Copies this state.
     *
     * @return A new CompletedState, owned by the caller.
     */
    TaskState* clone() const override;
};

#endif
