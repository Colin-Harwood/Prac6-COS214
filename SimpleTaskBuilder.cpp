#include "SimpleTaskBuilder.h"
#include "SimpleTask.h"

SimpleTaskBuilder::SimpleTaskBuilder(){
    task = new SimpleTask("", "", 0);
}

SimpleTaskBuilder::~SimpleTaskBuilder() {
}

void SimpleTaskBuilder::setName(std::string name) {
    //  setting name on task
    task->name = name;
    task->deptLevel = Task::parseDeptLevel(name);
}

void SimpleTaskBuilder::setDeadline(std::string deadline) {
    //  setting deadline on task
    task->deadline = deadline;
}

void SimpleTaskBuilder::setDeptLevel(int deptLevel) {
    //  setting dept level on task
    task->deptLevel = deptLevel;
}

void SimpleTaskBuilder::setDuration(int duration) {
    //  setting duration on task
    task->duration = duration;
}

void SimpleTaskBuilder::setComplete(bool complete) {
    //  setting complete status on task
    task->isComplete = complete;
}

void SimpleTaskBuilder::setState(TaskState* state) {
    //  setting state on composite task
    task->state = state;
}

void SimpleTaskBuilder::setChildren(std::vector<Task*> children) {
    // SimpleTask is a leaf node; children are not supported
}

SimpleTask* SimpleTaskBuilder::getResult() {
    return task;
}