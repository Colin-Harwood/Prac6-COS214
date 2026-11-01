#ifndef DEPARTMENTTASKITERATOR_H
#define DEPARTMENTTASKITERATOR_H

#include <vector>
#include "TaskIterator.h"

class Task;

class DepartmentTaskIterator : public TaskIterator {
private:
    Task* root;
    std::vector<Task*> flatTasks;
    int currentIndex;
    int targetDepartment;

    /**
     * @brief Recursively flattens the composite Task hierarchy into flatTasks.
     * @param node Current Task node being visited.
     */
    void flatComposite(Task* node);

    /**
     * @brief Determines whether a Task matches the filter criteria for this iterator.
     * @param t Pointer to the Task to evaluate.
     * @return true if t is a non-null leaf task matching targetDepartment and not yet finished.
     */
    bool isEligible(Task* t);

public:
    /**
     * @brief Constructs a DepartmentTaskIterator for a given root Task and department level.
     * @param root Pointer to the root Task (SimpleTask or TaskGroup) to traverse.
     * @param deptLevel Target department category number to filter by.
     */
    DepartmentTaskIterator(Task* root, int deptLevel);

    ~DepartmentTaskIterator() override = default;

    /**
     * @brief Positions the iterator at the first eligible unfinished leaf task.
     */
    void first() override;

    /**
     * @brief Advances the iterator to the next eligible unfinished leaf task.
     */
    void next() override;

    /**
     * @brief Checks whether the iterator has passed the last eligible task in flatTasks.
     * @return true if currentIndex is out of bounds, false otherwise.
     */
    bool isDone() override;

    /**
     * @brief Returns the eligible Task at the current iterator index.
     * @return Pointer to the current Task, or nullptr if isDone() is true.
     */
    Task* currentItem() override;

    /**
     * @brief Checks whether every leaf task in the flattened hierarchy has finished executing.
     * @return true if all leaf tasks in flatTasks report isFinished() == true, false otherwise.
     */
    bool allCompleted() override;
};

#endif // DEPARTMENTTASKITERATOR_H