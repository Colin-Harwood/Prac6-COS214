/**
 * @file PendingState.h
 * @brief Concrete state: a task that has not started yet and is ready to run.
 *
 * Role in the State pattern: ConcreteState.
 */
#ifndef PENDINGSTATE_H
#define PENDINGSTATE_H

#include "TaskState.h"

/**
 * @brief A task that has not started yet and is ready to run.
 */
class PendingState : public TaskState {
public:
    /**
     * @brief Starts the task.
     *
     * @param t Task the action applies to (not used).
     * @return A new InProgressState.
     */
    TaskState* start(Task* t) override;
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
     * @return "Pending".
     */
    std::string getName() const override;
    /**
     * @brief Copies this state.
     *
     * @return A new PendingState, owned by the caller.
     */
    TaskState* clone() const override;
};

#endif
