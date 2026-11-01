#include "Builder.h"

Builder::Builder() {
}

Builder::~Builder() {
}

void Builder::setName(std::string name) {
}

void Builder::setDeadline(std::string deadline) {
}

void Builder::setDuration(int duration) {
}

void Builder::setChildren(std::vector<Task*> children) {
}

void Builder::setDeptLevel(int deptLevel) {
}

void Builder::setComplete(bool complete) {
}

void Builder::setState(TaskState* state) {
}

Task* Builder::getResult() {
    return nullptr;
}