#ifndef BUILDER_H
#define BUILDER_H

#include <string>
#include <vector>

// Forward declaration
class Task;
class TaskState;

class Builder {
public:
    Builder();
    virtual ~Builder();
    virtual void setName(std::string name);
    virtual void setDeadline(std::string deadline);
    virtual void setDuration(int duration);
    virtual void setChildren(std::vector<Task*> children);
    virtual void setDeptLevel(int deptLevel);
    virtual void setComplete(bool complete);
    virtual void setState(TaskState* state);

    virtual Task* getResult();
};

#endif // BUILDER_H