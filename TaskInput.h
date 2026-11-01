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
    /// Task name.
    std::string name;
    /// Task deadline.
    std::string deadline;
    /// Department level associated with the task.
    int deptLevel = 0;
    /// State to assign to the task.
    TaskState* state = nullptr;
    /// Whether this input describes a task group rather than a simple task.
    bool isGroup = false;
    /// Duration of a simple task.
    int duration = 0;
    /// Whether the task is complete.
    bool complete = false;
    /// Child task descriptions; used for groups.
    std::vector<TaskInput> children;
};

#endif // TASKINPUT_H
