#ifndef TASKINPUT_H
#define TASKINPUT_H

#include <string>
#include <vector>

class TaskState;

struct TaskInput {
    std::string name;
    std::string deadline;
    int deptLevel = 0;
    TaskState* state = nullptr;
    bool isGroup = false;
    int duration = 0;
    bool complete = false;
    std::vector<TaskInput> children;
};

#endif // TASKINPUT_H
