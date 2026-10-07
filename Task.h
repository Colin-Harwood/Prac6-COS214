#ifndef TASK_H
#define TASK_H

#include <memory>
#include <string>
#include <vector>

class Command;
class Status;
class PriorityStrategy;
class Observer;
class Snapshot;
class Iterator;

//  Task
//  Chain of Responsibility : Handler (holds #next, handle() runs then passes on)
//  Composite : Component (add/remove/totalTime/iterators)
//  Observer : Subject (attach/detach/notify)
//  Memento : Originator (createSnapshot/setSnapshot)
//  Command : Receiver
//  State : Context (status)
//  Strategy : Context (priority)
 
class Task {
protected:
    std::shared_ptr<Task> next;
    std::string name_;
    Command* command;
    std::string id;
    Status* status;
    int timer;
    std::string user;
    PriorityStrategy* priority;
    std::vector<Observer*> observerList;
public:
    Task();
    virtual ~Task();

    //Builder support
    void setId(const std::string& newId);
    void setName(const std::string& newName);
    void setTimer(int t);
    void setUser(const std::string& u);

    //Chain of Responsibility
    void setNext(std::shared_ptr<Task> next);
    virtual void handle();
    virtual void execute();
    virtual void undo();

    //Composite
    virtual void add(std::shared_ptr<Task> t);
    virtual void remove(const Task* t);
    virtual std::string name() = 0;
    virtual int totalTime() = 0;
    std::string getId();

    //State
    void advanceStatus();
    void setStatus(Status* s);
    Status* getStatus();
    void cancel();

    //Command
    void setCommand(Command* c);

    //Strategy
    void setPriority(PriorityStrategy* p);
    void route();
    void escalate();
    std::string getPriorityLevel();
    void assignTo(const std::string& person);

    //Time/Observer
    int getTimer();
    void tick(int elapsed);
    void attach(Observer* observer);
    void detach(Observer* observer);
    void notify();

    //Iterator factory methods
    virtual Iterator* createAllIterator() = 0;
    virtual Iterator* createMedIterator() = 0;
    virtual Iterator* createRushIterator() = 0;

    //Memento
    Snapshot* createSnapshot();
    void setSnapshot(Snapshot* snapshot);

    // Snapshot
    friend class Snapshot;
};

#endif