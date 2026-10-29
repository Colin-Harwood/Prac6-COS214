#include "TaskGroupBuilder.h"
#include "TaskGroup.h"

TaskGroupBuilder::TaskGroupBuilder() : task(nullptr) {
}

TaskGroupBuilder::~TaskGroupBuilder() {
}

void TaskGroupBuilder::setName(std::string name) {
    // Implement setting name on composite task
    task.name = name;

}

void TaskGroupBuilder::setDeadline(std::string deadline) {
    // Implement setting deadline on composite task
    task.deadline = deadline;
}

void TaskGroupBuilder::setDeptLevel(int deptLevel) {
    // Implement setting dept level on composite task
    task.deptLevel = deptLevel;
}

void TaskGroupBuilder::setComplete(bool complete) {
    // Implement setting complete status on composite task
    task.complete = false;

}

void TaskGroupBuilder::setState(TaskState* state) {
    // Implement setting state on composite task
    task.state = state
}

void TaskGroupBuilder::setChildren(std::vector<Task*> children) {
    // Implement adding child tasks to composite task
    
}

TaskGroup* TaskGroupBuilder::getResult() {
    return task;
}