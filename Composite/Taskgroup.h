#ifndef TASKGROUP_H
#define TASKGROUP_H

#include <vector>
#include "Task.h"

/**
 * @class TaskGroup
 * @brief Composite a task that contains subtasks (SimpleTask or other TaskGroups)
 */
class TaskGroup :public Task{
private:
    std::vector<Task*> children;
public:
    TaskGroup(std::string name, std::string code, std::string deadline, int deptLevel);
    ~TaskGroup() override;
    int totalTime() const override;
    void execute() override;
    void add(Task* t) override;
    void remove(Task* t) override;
    /** @return A deep copy of the group and all children */
    Task* clone() override;
    /** @return Iterator over children matching @p deptLevel. */
    TaskIterator* createIterator(int deptLevel) override;
};

#endif