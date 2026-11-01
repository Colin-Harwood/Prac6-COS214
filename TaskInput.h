/**
 * @file TaskInput.h
 * @brief Data structure holding input parameters for building tasks.
 */

#ifndef TASKINPUT_H
#define TASKINPUT_H

#include <string>
#include <vector>

class TaskState;

/**
 * @brief Describes a task and, if needed, its child tasks for construction.
 *
 * A TaskInput isGroup value determines whether the director creates a task
 * group or a simple leaf task. Group inputs can contain nested children.
 */
struct TaskInput {
    /// @brief Task name.
    std::string name;
    /// @brief Task deadline.
    std::string deadline;
    /// @brief Department level associated with the task.
    int deptLevel = 0;
    /// @brief State to assign to the task.
    TaskState* state = nullptr;
    /// @brief Whether this input describes a task group rather than a simple task.
    bool isGroup = false;
    /// @brief Duration of a simple task.
    int duration = 0;
    /// @brief Whether the task is complete.
    bool complete = false;
    /// @brief Child task descriptions; used for groups.
    std::vector<TaskInput> children;
};

#endif // TASKINPUT_H
