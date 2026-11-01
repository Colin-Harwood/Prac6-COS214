/**
 * @file TaskGroupBuilder.h
 * @brief Builder for creating TaskGroup objects.
 */

#ifndef TASKGROUPBUILDER_H
#define TASKGROUPBUILDER_H

#include "Builder.h"
#include "TaskGroup.h"

/**
 * @brief Builds a TaskGroup from task properties and child tasks.
 */
class TaskGroupBuilder : public Builder {
private:
    TaskGroup* task;

public:
    /// @brief Creates a builder with a new, initially empty TaskGroup.
    TaskGroupBuilder();
    /// @brief Destroys the builder.
    virtual ~TaskGroupBuilder();

    /// @brief Sets the task group name.
    void setName(std::string name) override;
    /// @brief Sets the task group deadline.
    void setDeadline(std::string deadline) override;
    /// @brief Sets the task group's department level.
    void setDeptLevel(int deptLevel) override;
    /// @brief Sets whether the task group is complete.
    void setComplete(bool complete) override;
    /// @brief Sets the task group's state.
    void setState(TaskState* state) override;
    /// @brief Adds the given tasks as children of the group.
    void setChildren(std::vector<Task*> children) override;

    /// @brief Returns the TaskGroup being built.
    TaskGroup* getResult() override;
};

#endif // TASKGROUPBUILDER_H
