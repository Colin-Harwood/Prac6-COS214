#ifndef SIMPLETASKBUILDER_H
#define SIMPLETASKBUILDER_H

#include "Builder.h"
#include "SimpleTask.h"

/**
 * @brief Builds a SimpleTask from task properties.
 *
 * SimpleTask is a leaf task and does not support child tasks.
 */
class SimpleTaskBuilder : public Builder {
private:
    SimpleTask* task;

public:
    /// Creates a builder with a new, initially empty SimpleTask.
    SimpleTaskBuilder();
    /// Destroys the builder.
    virtual ~SimpleTaskBuilder();

    /// Sets the task name.
    void setName(std::string name) override;
    /// Sets the task deadline.
    void setDeadline(std::string deadline) override;
    /// Sets the task's department level.
    void setDeptLevel(int deptLevel) override;
    /// Sets the task duration.
    void setDuration(int duration) override;
    /// Sets whether the task is complete.
    void setComplete(bool complete) override;
    /// Sets the task state.
    void setState(TaskState* state) override;

    /// Does nothing because a simple task is a leaf node.
    void setChildren(std::vector<Task*> children) override;

    /// Returns the SimpleTask being built.
    SimpleTask* getResult() override;
};

#endif // SIMPLETASKBUILDER_H