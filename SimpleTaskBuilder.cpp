#include "SimpleTaskBuilder.h"
#include "SimpleTask.h"

SimpleTaskBuilder::SimpleTaskBuilder() : task(nullptr) {
}

SimpleTaskBuilder::~SimpleTaskBuilder() {
}

void SimpleTaskBuilder::setName(std::string name) {
    // Implement setting name on task
    task.name = name;
}

void SimpleTaskBuilder::setDeadline(std::string deadline) {
    // Implement setting deadline on task
    task.deadline = deadline;
}

void SimpleTaskBuilder::setDeptLevel(int deptLevel) {
    // Implement setting dept level on task
    task.deptLevel = deptLevel;
}

void SimpleTaskBuilder::setDuration(int duration) {
    // Implement setting duration on task
    task.duration = duration;
}

void SimpleTaskBuilder::setComplete(bool complete) {
    // Implement setting complete status on task
    task.complete = false;
}

void SimpleTaskBuilder::setState(TaskState* state) {
    // Implement setting state on composite task
    task.state = state
}

void SimpleTaskBuilder::setChildren(std::vector<Task*> children) {
    // SimpleTask is a leaf node; children are not supported
}

SimpleTask* SimpleTaskBuilder::getResult() {
    return task;
}