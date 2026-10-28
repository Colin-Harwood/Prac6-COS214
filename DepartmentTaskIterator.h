#ifndef DEPARTMENTTASKITERATOR_H
#define DEPARTMENTTASKITERATOR_H

#include <vector>
#include "TaskIterator.h"

class Task;

class DepartmentTaskIterator : public TaskIterator
{
private:
    Task *root;
    std::vector<Task *> flatTasks;
    int currentIndex;
    int targetDepartment;
    void flatComposite(Task *node);
    bool isEligible(Task *t);

public:
    DepartmentTaskIterator(Task *root, int deptLevel);
    void first() override;
    void next() override;
    bool isDone() override;
    Task *currentItem() override;
    bool allCompleted() override;
};

#endif