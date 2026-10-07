#ifndef TASKGROUP_H
#define TASKGROUP_H
// This is the .h of the TaskGroup class it forms part as the
// Composite participant in the Composite design pattern
#include <memory>
#include <string>
#include <vector>
#include "Task.h"


class TaskGroup : public Task {
private:
    std::vector<std::shared_ptr<Task>> children;
public:
    TaskGroup();
    explicit TaskGroup(const std::string& name);
    ~TaskGroup() override;
    void add(std::shared_ptr<Task> t) override;
    void remove(const Task* t) override;
    void execute() override;
    void undo() override;
    std::string name() override;
    int totalTime() override;
    Iterator* createAllIterator() override;
    Iterator* createMedIterator() override;
    Iterator* createRushIterator() override;
};

#endif // TASKGROUP_H