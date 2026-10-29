#ifndef TASKGROUPBUILDER_H
#define TASKGROUPBUILDER_H

#include "Builder.h"

class TaskGroupBuilder : public Builder {
private:
    TaskGroup* task;

public:
    TaskGroupBuilder();
    virtual ~TaskGroupBuilder();

    void setName(std::string name) override;
    void setDeadline(std::string deadline) override;
    void setDeptLevel(int deptLevel) override;
    void setComplete(bool complete) override;
    void setState(TaskState* state) override;
    void setChildren(std::vector<Task*> children) override;

    TaskGroup* getResult() override;
};

#endif // TASKGROUPBUILDER_H