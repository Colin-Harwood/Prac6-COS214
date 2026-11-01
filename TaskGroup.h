#ifndef TASKGROUP_H
#define TASKGROUP_H

#include "Task.h"
#include <vector>
class TaskGroupBuilder;
class TaskGroup : public Task {
    friend class TaskGroupBuilder;
private:
    std::vector<Task*> children;//vector of simpletasks
public:
    TaskGroup(const std::string& name, const std::string& deadline);
    ~TaskGroup() override;
    int totalTime() const override;
    void execute() override;
    void add(Task* t) override;
    void remove(Task* t) override;
    bool isLeaf() const override;
    std::vector<Task*> getChildren() const override;//add to UML
    TaskGroup(const TaskGroup& other);
    // Prototype
    Task* clone() const override;
    TaskIterator* createIterator(int level) override;
};

#endif
