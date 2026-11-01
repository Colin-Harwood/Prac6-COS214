/**
 * @file Builder.h
 * @brief Base Builder interface for constructing Task objects.
 */

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
    /// @brief Constructs a base builder.
    Builder();
    /// @brief Destroys the builder.
    virtual ~Builder();
    /// @brief Sets the task name.
    virtual void setName(std::string name);
    /// @brief Sets the task deadline.
    virtual void setDeadline(std::string deadline);
    /// @brief Sets the task duration.
    virtual void setDuration(int duration);
    /// @brief Sets the task's children.
    virtual void setChildren(std::vector<Task*> children);
    /// @brief Sets the task's department level.
    virtual void setDeptLevel(int deptLevel);
    /// @brief Sets whether the task is complete.
    virtual void setComplete(bool complete);
    /// @brief Sets the task state.
    virtual void setState(TaskState* state);

    /// @brief Returns the task being built, or nullptr in the base implementation.
    virtual Task* getResult();
};

#endif // BUILDER_H
