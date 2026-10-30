#include "Director.h"
#include "SimpleTaskBuilder.h"
#include "TaskGroupBuilder.h"
#include "TaskInput.h"

#include <stdexcept>

namespace {
Task* buildTask(const TaskInput& input, Builder& builder) {
    if (input.isGroup) {
        // Match each input node with a builder that can create its concrete task type.
        auto* groupBuilder = dynamic_cast<TaskGroupBuilder*>(&builder);
        if (!groupBuilder) {
            throw std::invalid_argument("Group input requires a TaskGroupBuilder");
        }

        builder.setName(input.name);
        builder.setDeadline(input.deadline);
        builder.setDeptLevel(input.deptLevel);
        builder.setComplete(input.complete);
        builder.setState(input.state);

        std::vector<Task*> children;
        children.reserve(input.children.size());
        for (const TaskInput& childInput : input.children) {
            // Build descendants first so the completed child tasks can be attached to this group.
            if (childInput.isGroup) {
                TaskGroupBuilder childBuilder;
                children.push_back(buildTask(childInput, childBuilder));
            } else {
                SimpleTaskBuilder childBuilder;
                children.push_back(buildTask(childInput, childBuilder));
            }
        }
        groupBuilder->setChildren(children);
    } else {
        // A leaf must use the simple-task builder and cannot have descendants.
        auto* simpleTaskBuilder = dynamic_cast<SimpleTaskBuilder*>(&builder);
        if (!simpleTaskBuilder) {
            throw std::invalid_argument("Simple task input requires a SimpleTaskBuilder");
        }
        if (!input.children.empty()) {
            throw std::invalid_argument("Simple task input cannot contain children");
        }

        builder.setName(input.name);
        builder.setDeadline(input.deadline);
        builder.setDeptLevel(input.deptLevel);
        builder.setDuration(input.duration);
        builder.setComplete(input.complete);
        builder.setState(input.state);
    }

    // Return the task only after all of its properties and, for groups, children are set.
    return builder.getResult();
}
}

Director::Director() : builder(nullptr), input(nullptr) {}

Director::Director(TaskInput* input, Builder* builder) 
    : builder(builder), input(input) {}

Director::~Director() {
}

void Director::constructTask() {
    if (!builder || !input) {
        throw std::logic_error("Director requires both a builder and task input");
    }

    // Start recursive construction from the root input using the caller-provided builder.
    buildTask(*input, *builder);
}

void Director::setBuilder(Builder* b) {
    this->builder = b;
}
