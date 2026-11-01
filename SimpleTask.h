#ifndef SIMPLETASK_H
#define SIMPLETASK_H

#include "Task.h"

class SimpleTask : public Task {
    friend class SimpleTaskBuilder;
private:
    int duration;
public:
    SimpleTask(const std::string& name, const std::string& deadline, int duration);
    int totalTime() const override;
    void execute() override;
    bool isLeaf() const override;

    // Prototype
    SimpleTask(const SimpleTask& other);
    Task* clone() const override;
    TaskIterator* createIterator(int level) override;
};

#endif
