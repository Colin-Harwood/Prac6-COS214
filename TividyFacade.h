#ifndef TIVIDYFACADE_H
#define TIVIDYFACADE_H

#include <string>
#include <vector>
#include "TaskInput.h"

class Director;
class Task;
class Handler;
class Command;
class LogCommand;
class LogQueue;
class TaskSnapshotManager;
class Target;

class TividyFacade {
private:
    Director* director;
    Task* root;
    Handler* chain;
    LogQueue* logQueue;
    TaskSnapshotManager* snapshotManager;
    Target* notifier;

    std::vector<Handler*> handlers;
    Command* command;
    LogCommand* logCommand;
    std::vector<TaskInput> pending;
    std::vector<Task*> undoTargets;

    void buildChain();
    Task* findTask(const std::string& taskName);

    Task* findIn(Task* node, const std::string& taskName);
    void collectLevels(Task* node, std::vector<int>& levels);
    void collectLeaves(Task* node, std::vector<Task*>& leaves);
    void printNode(Task* node, int depth);
    int dispatchLevel(int level);
    void sendCompletionEmail();
    bool transition(const std::string& taskName, void (Task::*move)(), const std::string& action);
    void closeLogIfFinished();

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

    void setRoot(Task* t);

    bool beginWorkflow(const std::string& name, const std::string& deadline);
    bool beginGroup(const std::string& name, const std::string& deadline);
    bool addTask(const std::string& name, const std::string& deadline, int deptLevel, int duration);
    bool endGroup();
    bool finishWorkflow();

    void setNotifier(Target* t);
    bool setNotificationEmail(const std::string& email);
};

#endif
