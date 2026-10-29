#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>

class TaskState;
class TaskIterator;
class SimpleTaskBuilder;
class TaskGroupBuilder;
/**
 * Task = Component (Composite)
 * The name starts with its category number
 * That number is stored in deptLevel and is what the handlers look at
 */
class Task {
    friend class SimpleTaskBuilder;
    friend class TaskGroupBuilder;
private:
    std::string name;
    std::string deadline;
    int deptLevel;
    bool isComplete;
    TaskState* state;
    static int parseDeptLevel(const std::string& taskName);//add to UML
public:
    Task(const std::string& name, const std::string& deadline);
    virtual ~Task();
    std::string getName() const;
    std::string getDeadline() const;
    int getDepartmentLevel() const;

    //Composite
    virtual int totalTime() const = 0;
    virtual void execute() = 0;
    virtual void add(Task* t);
    virtual void remove(Task* t);
    virtual bool isLeaf() const = 0;
    virtual std::vector<Task*> getChildren() const;

    //Prototype
    virtual Task* clone() const = 0;

    //Iterator
    virtual TaskIterator* createIterator(int level) = 0;

    //State
    void setCompleteStatus(bool b);
    void setState(TaskState* s);
    std::string getStateName() const;
    void start();
    void complete();
    void reject();
    void timeout();
    void cancel();

    ///ADDED BY KAI
    void restoreFrom(Task* snapshot);
};

#endif
