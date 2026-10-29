#ifndef TASKITERATOR_H
#define TASKITERATOR_H

class Task;

class TaskIterator {
public:
    virtual ~TaskIterator() {}
    virtual void first() = 0;
    virtual void next() = 0;
    virtual bool isDone() = 0;
    virtual Task* currentItem() = 0;
    virtual bool allCompleted() = 0;
};

#endif // TASKITERATOR_H