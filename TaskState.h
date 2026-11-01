/**
 * @file TaskState.h
 * @brief State interface for tasks, and the error for a refused move.
 *
 * Role in the State pattern: State.
 */
#ifndef TASKSTATE_H
#define TASKSTATE_H

#include <stdexcept>
#include <string>

class Task;

/**
 * @brief Thrown when a task's current state does not allow an action.
 */
class InvalidTransition : public std::logic_error {
public:
    /**
     * @brief Builds a message such as "Cannot start a task that is Completed".
     *
     * @param stateName Name of the state that refused the action.
     * @param action The action that was refused.
     */
    InvalidTransition(const std::string& stateName, const std::string& action);
};

/**
 * @brief Base class for the state of a task.
 *
 * Every action is refused by default, so a concrete state only overrides the moves it allows.
 */
class TaskState {
public:
    /**
     * @brief Virtual because tasks delete states through a TaskState pointer.
     */
    virtual ~TaskState();

    /**
     * @brief Starts the task.
     *
     * @param t Task the action applies to.
     * @return The next state, which the task takes ownership of.
     * @throws InvalidTransition if this state does not allow the action.
     */
    virtual TaskState* start(Task* t);
    /**
     * @brief Completes the task.
     *
     * @param t Task the action applies to.
     * @return The next state, which the task takes ownership of.
     * @throws InvalidTransition if this state does not allow the action.
     */
    virtual TaskState* complete(Task* t);
    /**
     * @brief Rejects the task.
     *
     * @param t Task the action applies to.
     * @return The next state, which the task takes ownership of.
     * @throws InvalidTransition if this state does not allow the action.
     */
    virtual TaskState* reject(Task* t);
    /**
     * @brief Marks the task as timed out.
     *
     * @param t Task the action applies to.
     * @return The next state, which the task takes ownership of.
     * @throws InvalidTransition if this state does not allow the action.
     */
    virtual TaskState* timeout(Task* t);
    /**
     * @brief Cancels the task.
     *
     * @param t Task the action applies to.
     * @return The next state, which the task takes ownership of.
     * @throws InvalidTransition if this state does not allow the action.
     */
    virtual TaskState* cancel(Task* t);
    /**
     * @brief Reopens the task.
     *
     * @param t Task the action applies to.
     * @return The next state, which the task takes ownership of.
     * @throws InvalidTransition if this state does not allow the action.
     */
    virtual TaskState* reopen(Task* t);

    /**
     * @brief Gets the name of this state.
     *
     * @return The state name, such as "Pending".
     */
    virtual std::string getName() const = 0;
    /**
     * @brief Checks whether this state counts as finished.
     *
     * @return False by default.
     */
    virtual bool isFinished() const;

    /**
     * @brief Copies this state, so a copied task has its own state object.
     *
     * @return A new state, owned by the caller.
     */
    virtual TaskState* clone() const = 0;
};

#endif
