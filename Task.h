#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>

class TaskState;
class TaskIterator;
class SimpleTaskBuilder;
class TaskGroupBuilder;
class Observer;
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

    std::vector<Observer*> observers;   // Observer

    static int parseDeptLevel(const std::string& taskName);//add to UML
public:
    Task(const std::string& name, const std::string& deadline);
    virtual ~Task();
    virtual std::string getName() const;
    virtual std::string getDeadline() const;
    virtual int getDepartmentLevel() const;

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
    virtual bool isFinished() const; // Added by Motheo

    //State
    Task(const Task& other);
    virtual void setCompleteStatus(bool b);
    virtual void setState(TaskState* s);
    virtual std::string getStateName() const;
    virtual void start();
    virtual void complete();
    virtual void reject();
    virtual void timeout();
    virtual void cancel();
    virtual void reopen();                      // State

    //Observer
    virtual void addObserver(Observer* observer);
    virtual void removeObserver(Observer* observer);
    virtual void notify(std::string type);

    ///ADDED BY KAI
    virtual void restoreFrom(Task* snapshot);
};

#endif