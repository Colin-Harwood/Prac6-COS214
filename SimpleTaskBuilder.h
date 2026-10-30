#ifndef SIMPLETASKBUILDER_H
#define SIMPLETASKBUILDER_H

#include "Builder.h"
#include "SimpleTask.h"

class SimpleTaskBuilder : public Builder {
private:
    SimpleTask* task;

public:
    SimpleTaskBuilder();
    virtual ~SimpleTaskBuilder();

    void setName(std::string name) override;
    void setDeadline(std::string deadline) override;
    void setDeptLevel(int deptLevel) override;
    void setDuration(int duration) override;
    void setComplete(bool complete) override;
    void setState(TaskState* state) override;

    //not actually gonna do anything sine leaf node
    void setChildren(std::vector<Task*> children) override;

    SimpleTask* getResult() override;
};

#endif // SIMPLETASKBUILDER_H