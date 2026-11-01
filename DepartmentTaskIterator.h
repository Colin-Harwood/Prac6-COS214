/**
 * @file DepartmentTaskIterator.h
 * @brief Iterator for traversing tasks filtered by department level.
 */

#ifndef DEPARTMENTTASKITERATOR_H
#define DEPARTMENTTASKITERATOR_H

#include <vector>
#include "TaskIterator.h"

class Task;

/**
 * @class DepartmentTaskIterator
 * @brief Concrete iterator that flattens a composite Task tree and filters leaf tasks by department level.
 *
 * Flattens a composite Task hierarchy in pre-order and iterates exclusively over
 * unfinished leaf tasks whose department level matches targetDepartment.
 */
class DepartmentTaskIterator : public TaskIterator {
private:
    Task* root;                    /**< Root Task node of the composite hierarchy being traversed */
    std::vector<Task*> flatTasks;  /**< Flattened pre-order sequence of all Task nodes in the tree */
    int currentIndex;              /**< Current index position within flatTasks */
    int targetDepartment;          /**< Target department category level used to filter tasks */

    /**
     * @brief Recursively flattens the composite Task tree into the flatTasks vector.
     * @param node Pointer to the current Task node being visited.
     */
    void flatComposite(Task* node);

    /**
     * @brief Determines whether a Task node qualifies to be visited during iteration.
     * @param t Pointer to the Task node under evaluation.
     * @return true if t is a non-null unfinished leaf task matching targetDepartment, false otherwise.
     */
    bool isEligible(Task* t);

public:
    /**
     * @brief Constructs a DepartmentTaskIterator for a given root Task and department category.
     * @param root Pointer to the root Task of the composite structure.
     * @param deptLevel Department category number to filter leaf tasks by.
     */
    DepartmentTaskIterator(Task* root, int deptLevel);

    /**
     * @brief Default virtual destructor.
     */
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
     * @brief Checks if the traversal has reached the end of flatTasks.
     * @return true if currentIndex is out of bounds, false otherwise.
     */
    bool isDone() override;

    /**
     * @brief Returns the eligible Task at the current iterator index.
     * @return Pointer to the current Task, or nullptr if isDone() is true.
     */
    Task* currentItem() override;

    /**
     * @brief Verifies whether all leaf tasks in the flattened composite tree are finished.
     * @return true if every leaf task in flatTasks reports isFinished() == true, false otherwise.
     */
    bool allCompleted() override;
};

#endif // DEPARTMENTTASKITERATOR_H
