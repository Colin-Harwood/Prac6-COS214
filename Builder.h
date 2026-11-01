#ifndef BUILDER_H
#define BUILDER_H

#include <string>
#include <vector>

// Forward declarations
class Task;
class TaskState;

/**
 * @brief Base interface for constructing Task objects.
 *
 * The default implementations do nothing and getResult() returns nullptr.
 * Concrete builders override the operations they support.
 */
class Builder {
public:
    /// Constructs a base builder.
    Builder();
    /// Destroys the builder.
    virtual ~Builder();
    /// Sets the task name.
    virtual void setName(std::string name);
    /// Sets the task deadline.
    virtual void setDeadline(std::string deadline);
    /// Sets the task duration.
    virtual void setDuration(int duration);
    /// Sets the task's children.
    virtual void setChildren(std::vector<Task*> children);
    /// Sets the task's department level.
    virtual void setDeptLevel(int deptLevel);
    /// Sets whether the task is complete.
    virtual void setComplete(bool complete);
    /// Sets the task state.
    virtual void setState(TaskState* state);

    /// Returns the task being built, or nullptr in the base implementation.
    virtual Task* getResult();
};

#endif // BUILDER_H