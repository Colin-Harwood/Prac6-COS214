#ifndef TASKITERATOR_H
#define TASKITERATOR_H

class Task;

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
     * @brief Checks whether the traversal has reached the end of the collection.
     * @return true if no more eligible tasks remain, false otherwise.
     */
    virtual bool isDone() = 0;

    /**
     * @brief Retrieves the Task at the current iterator position.
     * @return Pointer to the current Task, or nullptr if isDone() is true.
     */
    virtual Task* currentItem() = 0;
    virtual bool allCompleted() = 0;
};

#endif // TASKITERATOR_H