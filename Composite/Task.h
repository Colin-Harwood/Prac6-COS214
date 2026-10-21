#ifndef TASK_H
#define TASK_H

#include <string>

class TaskIterator;

/**
 * @file Task.h
 * @brief Component particapant of the Composite design pattern.
 * Tasks will be stored as the composite design pattern with subtasks
 */


class Task{
protected:
    std::string name;     
    std::string code;     
    std::string deadline;  
    int deptLevel;        
public:
    Task(std::string name, std::string code, std::string deadline, int deptLevel);
    virtual ~Task();
    std::string getName() const;
    std::string getCode() const;
    std::string getDeadline() const;
    int getDepartmentLevel() const;
    virtual int totalTime() const = 0;
    virtual void execute() = 0;
    virtual void add(Task* t) = 0;
    virtual void remove(Task* t) = 0;
    /** @brief Prototype: creates a deep copy of this task */
    virtual Task* clone() = 0;
    /**
     * @brief Creates an iterator over tasks at a given department level
     * @return A new TaskIterator (caller owns it)
     */
    virtual TaskIterator* createIterator(int deptLevel) = 0;
    /**
     * @brief Serialises this task (and subtasks) into the markup string sent into the Chain of Responsibility
     */
    std::string generateCode();
};

#endif