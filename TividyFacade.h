// ==================================================================
//  COPY 1 of 2 - UML VERSION (matches the class diagram exactly)
// ==================================================================
#ifndef TIVIDYFACADE_H
#define TIVIDYFACADE_H

#include <string>
#include <vector>

class Director;
class Task;
class Handler;
class Command;
class LogQueue;
class TaskSnapshotManager;
class Target;
class Observer;   // IObserver in the UML

/**
 * TividyFacade = Facade
 * The only class the TUI (Client) talks to. It hides the builder, the handler
 * chain, the iterator, the commands, the snapshots and the log behind a few calls.
 */
class TividyFacade {
private:
    Director* director;                 // set once the Builder is merged into dev
    Task* root;
    Handler* chain;
    LogQueue* logQueue;
    TaskSnapshotManager* snapshotManager;
    Target* notifier;                   // set once the Adapter is merged into dev
    std::vector<Observer*> monitors;

    // Not in the UML: what the facade needs to clean up and undo correctly
    std::vector<Handler*> handlers;     // Handler does not own its next link
    Command* command;                   // shared by the handlers
    std::vector<Task*> undoTargets;     // which task each kept snapshot belongs to

    void buildChain();
    void attachObservers(Task* t);
    Task* findTask(const std::string& taskName);

    // Not in the UML: shared helpers
    Task* findIn(Task* node, const std::string& taskName);
    void collectLevels(Task* node, std::vector<int>& levels);
    void collectLeaves(Task* node, std::vector<Task*>& leaves);
    void printNode(Task* node, int depth);
    bool dispatch(Task* t);
    bool transition(const std::string& taskName, void (Task::*move)());

public:
    TividyFacade();
    ~TividyFacade();

    bool loadWorkflow(const std::string& file);
    void runWorkflow();
    void printWorkflow();
    std::string getTaskStatus(const std::string& taskName);
    bool startTask(const std::string& taskName);
    bool completeTask(const std::string& taskName);
    bool rejectTask(const std::string& taskName);
    bool cancelTask(const std::string& taskName);
    bool reopenTask(const std::string& taskName);
    bool undoLast();
    void showLog();
    void checkTimeouts();

    // Not in the UML: lets the TUI and tests use a tree until the Builder is merged. Takes ownership.
    void setRoot(Task* t);
};

#endif
