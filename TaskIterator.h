#ifndef TASKITERATOR_H
#define TASKITERATOR_H

class Task;

/**
 * @class TaskIterator
 * @brief Abstract interface for traversing and inspecting composite Task hierarchies.
 *
 * Provides sequential traversal operations (first, next, isDone, currentItem)
 * and whole-hierarchy completion checks (allCompleted) without exposing the
 * underlying composite structure to client handlers.
 */
class TaskIterator {
public:
    /**
     * @brief Virtual destructor for safe polymorphic cleanup.
     */
    virtual ~TaskIterator() {}

    /**
     * @brief Resets the iterator to the first eligible Task in the traversal.
     */
    virtual void first() = 0;

    /**
     * @brief Advances the iterator to the next eligible Task in the traversal.
     */
    virtual void next() = 0;

    /**
     * @brief Checks whether the iterator has passed the end of the eligible tasks.
     * @return true if no more eligible tasks remain in the traversal, false otherwise.
     */
    virtual bool isDone() = 0;

    /**
     * @brief Retrieves the Task at the current iterator position.
     * @return Pointer to the current Task, or nullptr if isDone() is true.
     */
    virtual Task* currentItem() = 0;

    /**
     * @brief Checks whether all tasks in the traversed hierarchy have been completed.
     * @return true if every applicable task in the hierarchy is finished, false otherwise.
     */
    virtual bool allCompleted() = 0;
};

#endif // TASKITERATOR_H