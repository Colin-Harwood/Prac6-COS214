/**
 * @file TimedOutState.h
 * @brief Concrete state: a task that ran too long and can be retried.
 *
 * Role in the State pattern: ConcreteState.
 */
#ifndef TIMEDOUTSTATE_H
#define TIMEDOUTSTATE_H

#include "TaskState.h"

/**
 * @brief A task that ran too long and can be retried.
 */
class TimedOutState : public TaskState {
public:
    /**
     * @brief Restarts the task.
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
     * @return "TimedOut".
     */
    std::string getName() const override;
    /**
     * @brief Copies this state.
     *
     * @return A new TimedOutState, owned by the caller.
     */
    TaskState* clone() const override;
};

#endif
