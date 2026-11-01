/**
 * @file SimpleTask.h
 * @brief Leaf task implementation representing an individual unit of work.
 */

#ifndef SIMPLETASK_H
#define SIMPLETASK_H

#include "Task.h"

/**
 * @brief Leaf task representation with a specific duration.
 */
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
    /**
     * @brief Clones the SimpleTask (Prototype pattern).
     */
    Task* clone() const override;
    TaskIterator* createIterator(int level) override;
};

#endif
