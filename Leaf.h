#ifndef LEAF_H
#define LEAF_H
#include "Task.h"
//  Composite : Leaf (atomic unit of work, no children)
//  Chain of Resp : ConcreteHandler
//  Observer : the subject a TimeoutObserver watches

class Leaf : public Task {
private:
    int duration;
public:
    Leaf();
    Leaf(const std::string& name, int duration);
    ~Leaf() override;
    void execute() override;
    void undo() override;
    std::string name() override;
    int totalTime() override;
    Iterator* createAllIterator() override;
    Iterator* createMedIterator() override;
    Iterator* createRushIterator() override;
};

#endif