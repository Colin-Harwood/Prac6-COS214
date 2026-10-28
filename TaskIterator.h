#ifndef TASKITERATOR_H
#define TASKITERATOR_H

class Task;

class TaskIterator
{
public:
    virtual void first() = 0;
    virtual void next() = 0;
    virtual bool isDone() = 0;
    virtual Task *currentItem();
    virtual bool allCompleted();
    virtual ~TaskIterator();
};

#endif // TASKITERATOR