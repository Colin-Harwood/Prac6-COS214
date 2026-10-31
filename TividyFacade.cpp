#include "TividyFacade.h"
#include "Task.h"
#include "TaskState.h"
#include "Handler.h"
#include "ConcreteHandlerA.h"
#include "ConcreteHandlerB.h"
#include "ConcreteHandlerC.h"
#include "ExecuteCommand.h"
#include "LogCommand.h"
#include "Director.h"
#include "TaskGroupBuilder.h"
#include "Target.h"
#include "LogQueue.h"
#include "TaskSnapshotManager.h"
#include "DepartmentTaskIterator.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace {
class ActionLog : public Command {
public:
    ActionLog(LogQueue* q, TaskSnapshotManager* s) : Command(q, s) {}
    void execute(Task*) override {}
    void undo(Task*) override {}
    Command* clone() const override { return new ActionLog(*this); }
    void log(Task* t, const std::string& act) { record(t, act); }
};
}

TividyFacade::TividyFacade()
    : director(nullptr), root(nullptr), chain(nullptr),
      logQueue(new LogQueue()), snapshotManager(new TaskSnapshotManager()),
      notifier(nullptr), command(nullptr), logCommand(nullptr) {
    buildChain();
}

TividyFacade::~TividyFacade() {
    for (size_t i = 0; i < handlers.size(); i++) {
        delete handlers[i];
    }
    delete command;
    delete logCommand;
    delete notifier;
    delete root;
    delete snapshotManager;

    std::ostringstream discard;
    logQueue->flushTo(discard);
    delete logQueue;

}

void TividyFacade::buildChain() {
    command = new ExecuteCommand(logQueue, snapshotManager);
    handlers.push_back(new ConcreteHandlerA(command));
    handlers.push_back(new ConcreteHandlerB(command));
    handlers.push_back(new ConcreteHandlerC(command));
    logCommand = new LogCommand(logQueue, snapshotManager, "workflow.log");
    for (size_t i = 0; i < handlers.size(); i++) {
        handlers[i]->setLogCommand(logCommand);
        if (i + 1 < handlers.size()) {
            handlers[i]->setNext(handlers[i + 1]);
        }
    }
    chain = handlers[0];
}

bool TividyFacade::loadWorkflow(const std::string& file) {
    std::cout << "Cannot load \"" << file << "\": loading from a file is not supported yet." << std::endl;
    return false;
}

void TividyFacade::setRoot(Task* t) {
    delete root;
    root = t;
    undoTargets.clear();
}

void TividyFacade::runWorkflow() {
    if (!root) {
        std::cout << "No workflow loaded." << std::endl;
        return;
    }
    std::vector<int> levels;
    collectLevels(root, levels);
    std::sort(levels.begin(), levels.end());
    levels.erase(std::unique(levels.begin(), levels.end()), levels.end());

    int ran = 0;
    for (size_t i = 0; i < levels.size(); i++) {
        ran += dispatchLevel(levels[i]);
    }

    std::vector<Task*> leaves;
    collectLeaves(root, leaves);
    for (size_t i = 0; i < leaves.size(); i++) {
        if (!leaves[i]->isFinished()) {
            return;
        }
    }
    std::cout << "Workflow finished." << std::endl;
    if (ran > 0) {
        sendCompletionEmail();
    }
}

void TividyFacade::checkTimeouts() {
    if (!root) {
        std::cout << "No workflow loaded." << std::endl;
        return;
    }
    std::vector<Task*> leaves;
    collectLeaves(root, leaves);
    std::vector<int> levels;
    for (size_t i = 0; i < leaves.size(); i++) {
        if (leaves[i]->getStateName() == "TimedOut") {
            std::cout << "Retrying timed-out task: " << leaves[i]->getName() << std::endl;
            levels.push_back(leaves[i]->getDepartmentLevel());
        }
    }
    if (levels.empty()) {
        std::cout << "No timed-out tasks." << std::endl;
        return;
    }
    std::sort(levels.begin(), levels.end());
    levels.erase(std::unique(levels.begin(), levels.end()), levels.end());
    for (size_t i = 0; i < levels.size(); i++) {
        dispatchLevel(levels[i]);
    }
}

int TividyFacade::dispatchLevel(int level) {
    std::vector<Task*> batch;
    std::vector<std::string> before;
    DepartmentTaskIterator it(root, level);
    for (it.first(); !it.isDone(); it.next()) {
        batch.push_back(it.currentItem());
        before.push_back(it.currentItem()->getStateName());
    }
    if (batch.empty()) {
        return 0;
    }
    try {
        chain->handle(root, level);
    } catch (const std::exception& e) {
        std::cout << "Could not run level " << level << ": " << e.what() << std::endl;
    }
    for (size_t i = 0; i < batch.size(); i++) {
        if (batch[i]->getStateName() != before[i]) {
            undoTargets.push_back(batch[i]);
        }
    }
    return static_cast<int>(batch.size());
}

bool TividyFacade::beginWorkflow(const std::string& name, const std::string& deadline) {
    pending.clear();
    TaskInput workflow;
    workflow.name = name;
    workflow.deadline = deadline;
    workflow.isGroup = true;
    pending.push_back(workflow);
    return true;
}

bool TividyFacade::beginGroup(const std::string& name, const std::string& deadline) {
    if (pending.empty()) {
        std::cout << "Call beginWorkflow first." << std::endl;
        return false;
    }
    TaskInput group;
    group.name = name;
    group.deadline = deadline;
    group.isGroup = true;
    pending.push_back(group);
    return true;
}

bool TividyFacade::addTask(const std::string& name, const std::string& deadline, int deptLevel, int duration) {
    if (pending.empty()) {
        std::cout << "Call beginWorkflow first." << std::endl;
        return false;
    }
    if (deptLevel < 1 || deptLevel > 9) {
        std::cout << "Task level must be from 1 to 9." << std::endl;
        return false;
    }
    TaskInput task;
    task.name = name;
    task.deadline = deadline;
    task.deptLevel = deptLevel;
    task.duration = duration;
    pending.back().children.push_back(task);
    return true;
}

bool TividyFacade::endGroup() {
    if (pending.size() < 2) {
        std::cout << "There is no open group to end." << std::endl;
        return false;
    }
    TaskInput group = pending.back();
    pending.pop_back();
    pending.back().children.push_back(group);
    return true;
}

bool TividyFacade::finishWorkflow() {
    if (pending.size() != 1) {
        std::cout << "Start a workflow and close all its groups first." << std::endl;
        return false;
    }
    TaskGroupBuilder builder;
    director = new Director(&pending[0], &builder);
    director->constructTask();
    delete director;
    director = nullptr;

    setRoot(builder.getResult());
    pending.clear();
    return true;
}

void TividyFacade::setNotifier(Target* t) {
    if (t != notifier) {
        delete notifier;
    }
    notifier = t;
}

bool TividyFacade::setNotificationEmail(const std::string& email) {
    if (!notifier || email.empty()) {
        return false;
    }
    notifier->setRecipientEmail(email);
    return true;
}

void TividyFacade::sendCompletionEmail() {
    if (!notifier) {
        return;
    }
    std::ifstream file("workflow.log");
    std::ostringstream log;
    log << file.rdbuf();
    notifier->notifyProjectComplete(root, log.str());
}

void TividyFacade::printWorkflow() {
    if (!root) {
        std::cout << "No workflow loaded." << std::endl;
        return;
    }
    printNode(root, 0);
}

void TividyFacade::printNode(Task* node, int depth) {
    std::cout << std::string(depth * 2, ' ') << "[" << node->getStateName() << "] "
              << node->getName() << " (dept " << node->getDepartmentLevel() << ")" << std::endl;
    std::vector<Task*> children = node->getChildren();
    for (size_t i = 0; i < children.size(); i++) {
        printNode(children[i], depth + 1);
    }
}

std::string TividyFacade::getTaskStatus(const std::string& taskName) {
    Task* t = findTask(taskName);
    return t ? t->getStateName() : "Task not found";
}

void TividyFacade::showLog() {
    logCommand->flushTo();
    std::ifstream file("workflow.log");
    std::ostringstream log;
    log << file.rdbuf();
    if (log.str().empty()) {
        std::cout << "(log is empty)" << std::endl;
        return;
    }
    std::cout << log.str();
}

bool TividyFacade::transition(const std::string& taskName, void (Task::*move)(), const std::string& action) {
    Task* t = findTask(taskName);
    if (!t) {
        std::cout << "No task named \"" << taskName << "\"." << std::endl;
        return false;
    }
    snapshotManager->addSnapshot(t);
    try {
        (t->*move)();
    } catch (const InvalidTransition& e) {
        delete snapshotManager->getSnapshot();
        std::cout << e.what() << std::endl;
        return false;
    }
    undoTargets.push_back(t);
    ActionLog(logQueue, snapshotManager).log(t, action);
    closeLogIfFinished();
    return true;
}

void TividyFacade::closeLogIfFinished() {
    std::vector<Task*> leaves;
    collectLeaves(root, leaves);
    for (size_t i = 0; i < leaves.size(); i++) {
        if (!leaves[i]->isFinished()) {
            return;
        }
    }
    logCommand->execute(root);
    sendCompletionEmail();
}

bool TividyFacade::startTask(const std::string& taskName)  { return transition(taskName, &Task::start, "STARTED"); }
bool TividyFacade::rejectTask(const std::string& taskName) { return transition(taskName, &Task::reject, "REJECTED"); }
bool TividyFacade::cancelTask(const std::string& taskName) { return transition(taskName, &Task::cancel, "CANCELLED"); }
bool TividyFacade::reopenTask(const std::string& taskName) { return transition(taskName, &Task::reopen, "REOPENED"); }

bool TividyFacade::completeTask(const std::string& taskName) {
    Task* t = findTask(taskName);
    if (t && t->isLeaf() && (t->getStateName() == "Pending" || t->getStateName() == "TimedOut")) {
        try {
            command->execute(t);
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            return false;
        }
        undoTargets.push_back(t);
        closeLogIfFinished();
        return true;
    }
    return transition(taskName, &Task::complete, "COMPLETED");
}

bool TividyFacade::undoLast() {
    if (undoTargets.empty()) {
        std::cout << "Nothing to undo." << std::endl;
        return false;
    }
    Task* t = undoTargets.back();
    undoTargets.pop_back();
    return snapshotManager->restoreLatest(t);
}

Task* TividyFacade::findTask(const std::string& taskName) {
    return root ? findIn(root, taskName) : nullptr;
}

Task* TividyFacade::findIn(Task* node, const std::string& taskName) {
    if (node->getName() == taskName) {
        return node;
    }
    std::vector<Task*> children = node->getChildren();
    for (size_t i = 0; i < children.size(); i++) {
        Task* found = findIn(children[i], taskName);
        if (found) {
            return found;
        }
    }
    return nullptr;
}

void TividyFacade::collectLevels(Task* node, std::vector<int>& levels) {
    if (node->isLeaf()) {
        levels.push_back(node->getDepartmentLevel());
    }
    std::vector<Task*> children = node->getChildren();
    for (size_t i = 0; i < children.size(); i++) {
        collectLevels(children[i], levels);
    }
}

void TividyFacade::collectLeaves(Task* node, std::vector<Task*>& leaves) {
    if (node->isLeaf()) {
        leaves.push_back(node);
    }
    std::vector<Task*> children = node->getChildren();
    for (size_t i = 0; i < children.size(); i++) {
        collectLeaves(children[i], leaves);
    }
}
