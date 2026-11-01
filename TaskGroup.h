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
    // State comes from the children, not a stored state
    /**
     * @brief Works out the group's state from its children.
     *
     * @return Pending, InProgress, Completed or Cancelled.
     */
    std::string getStateName() const override;
    /**
     * @brief Checks whether the group has children and all of them are finished.
     *
     * @return True if every child is finished. False for an empty group.
     */
    bool isFinished() const override;
    TaskGroup(const TaskGroup& other);
    // Prototype
    Task* clone() const override;
    TaskIterator* createIterator(int level) override;
};

#endif
