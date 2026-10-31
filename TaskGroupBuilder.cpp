#include "TaskGroupBuilder.h"
#include "TaskGroup.h"

TaskGroupBuilder::TaskGroupBuilder() {
    task = new TaskGroup("", "");
}

TaskGroupBuilder::~TaskGroupBuilder() {
}

void TaskGroupBuilder::setName(std::string name) {
    //  setting name on composite task
    task->name = name;
    task->deptLevel = Task::parseDeptLevel(name);

}

void TaskGroupBuilder::setDeadline(std::string deadline) {
    //  setting deadline on composite task
    task->deadline = deadline;
}

void TaskGroupBuilder::setDeptLevel(int deptLevel) {
    //  setting dept level on composite task
    task->deptLevel = deptLevel;
}

void TaskGroupBuilder::setComplete(bool complete) {
    //  setting complete status on composite task
    task->isComplete = complete;

}

void TaskGroupBuilder::setState(TaskState* state) {
    //  setting state on composite task
    task->state = state;
}

void TaskGroupBuilder::setChildren(std::vector<Task*> children) {
    for (Task* child : children) {
        task->add(child);
    }
}

TaskGroup* TaskGroupBuilder::getResult() {
    return task;
}