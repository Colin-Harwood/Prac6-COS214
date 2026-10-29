// ==================================================================
//  COPY 1 of 2 - UML VERSION (matches the class diagram exactly)
// ==================================================================
#ifndef TASKSTATE_H
#define TASKSTATE_H

#include <stdexcept>
#include <string>

class Task;

/**
 * Thrown when a task is asked to make a move its current state does not allow,
 * e.g. completing a task that was never started.
 * Derives from std::exception so ExecuteCommand's catch block rolls the task back.
 */
class InvalidTransition : public std::logic_error {
public:
    InvalidTransition(const std::string& stateName, const std::string& action);
};

/**
 * TaskState = State (State pattern)
 * Task is the Context. Every transition throws by default; each concrete
 * state overrides only the moves it allows and returns the next state.
 * States never call setState themselves - Task swaps the returned state in.
 */
class TaskState {
public:
    virtual ~TaskState();

    virtual TaskState* start(Task* t);
    virtual TaskState* complete(Task* t);
    virtual TaskState* reject(Task* t);
    virtual TaskState* timeout(Task* t);
    virtual TaskState* cancel(Task* t);
    virtual TaskState* reopen(Task* t);

    virtual std::string getName() const = 0;
    virtual bool isFinished() const;

    // Prototype: snapshots need their own copy of the state
    virtual TaskState* clone() const = 0;
};

#endif
