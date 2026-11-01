/**
 * @file CancelledState.h
 * @brief Concrete state: a task that was deliberately dropped. This state is final.
 *
 * Role in the State pattern: ConcreteState.
 */
#ifndef CANCELLEDSTATE_H
#define CANCELLEDSTATE_H

#include "TaskState.h"

/**
 * @brief A task that was deliberately dropped. This state is final.
 */
class CancelledState : public TaskState {
public:
    /**
     * @brief Gets the name of this state.
     *
     * @return "Cancelled".
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
     * @return A new CancelledState, owned by the caller.
     */
    TaskState* clone() const override;
};

#endif
