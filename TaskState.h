#ifndef TASKSTATE_H
#define TASKSTATE_H

#include <stdexcept>
#include <string>

class Task;

class InvalidTransition : public std::logic_error {
public:
    InvalidTransition(const std::string& stateName, const std::string& action);
};

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

    virtual TaskState* clone() const = 0;
};

#endif
