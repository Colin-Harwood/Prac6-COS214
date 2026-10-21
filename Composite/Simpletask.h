#ifndef SIMPLETASK_H
#define SIMPLETASK_H

#include "Task.h"

/**
 * @class SimpleTask
 * @brief Leaf of the Composite pattern a task with no subtasks
 */
class SimpleTask :public Task{
private:
    int duration; 
public:
    SimpleTask(std::string name, std::string code, std::string deadline, int deptLevel, int duration);
    int totalTime() const override;
    void execute() override;
    void add(Task* t) override;
    void remove(Task* t) override;
    Task* clone() override;
    /** @return An iterator over this single task */
    TaskIterator* createIterator(int deptLevel) override;
};

#endif