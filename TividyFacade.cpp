// ==================================================================
//  COPY 1 of 2 - UML VERSION (matches the class diagram exactly)
// ==================================================================
#include "TividyFacade.h"
#include "Task.h"
#include "TaskState.h"
#include "Handler.h"
#include "ConcreteHandlerA.h"
#include "ConcreteHandlerB.h"
#include "ConcreteHandlerC.h"
#include "ExecuteCommand.h"
#include "LogQueue.h"
#include "TaskSnapshotManager.h"
#include "DepartmentTaskIterator.h"
#include "Observer.h"

#include <algorithm>
#include <iostream>
#include <sstream>

TividyFacade::TividyFacade()
    : director(nullptr), root(nullptr), chain(nullptr),
      logQueue(new LogQueue()), snapshotManager(new TaskSnapshotManager()),
      notifier(nullptr), command(nullptr) {
    buildChain();
}

TividyFacade::~TividyFacade() {
    for (size_t i = 0; i < monitors.size(); i++) {
        delete monitors[i];
    }
    for (size_t i = 0; i < handlers.size(); i++) {
        delete handlers[i];
    }
    delete command;
    delete root;
    delete snapshotManager;
    // LogQueue only frees its commands when flushed
    std::ostringstream discard;
    logQueue->flushTo(discard);
    delete logQueue;
    // director and notifier: owned here once the Builder and Adapter are merged
}

// ---------------- Setup ----------------

// Builds A (categories 1-3) -> B (4-6) -> C (7-9); every handler runs tasks through one ExecuteCommand
void TividyFacade::buildChain() {
    command = new ExecuteCommand(logQueue, snapshotManager);
    handlers.push_back(new ConcreteHandlerA(command));
    handlers.push_back(new ConcreteHandlerB(command));
    handlers.push_back(new ConcreteHandlerC(command));
    for (size_t i = 0; i + 1 < handlers.size(); i++) {
        handlers[i]->setNext(handlers[i + 1]);
    }
    chain = handlers[0];
}

// TimeoutMonitor does not inherit Observer yet, so there is nothing to attach in this version
void TividyFacade::attachObservers(Task*) {}

bool TividyFacade::loadWorkflow(const std::string& file) {
    if (!director) {
        std::cout << "Cannot load \"" << file << "\": the Builder/Director is not in dev yet." << std::endl;
        return false;
    }
    // Once Director can return the tree it built: setRoot(<built tree>);
    return false;
}

void TividyFacade::setRoot(Task* t) {
    delete root;
    root = t;
    undoTargets.clear();
    if (root) {
        attachObservers(root);
    }
}

// ---------------- Running ----------------

// Each department level is handed to the chain in turn; the iterator gives the unfinished tasks of that level
void TividyFacade::runWorkflow() {
    if (!root) {
        std::cout << "No workflow loaded." << std::endl;
        return;
    }
    std::vector<int> levels;
    collectLevels(root, levels);
    std::sort(levels.begin(), levels.end());
    levels.erase(std::unique(levels.begin(), levels.end()), levels.end());

    for (size_t i = 0; i < levels.size(); i++) {
        std::vector<Task*> batch;
        DepartmentTaskIterator it(root, levels[i]);
        for (it.first(); !it.isDone(); it.next()) {
            if (it.currentItem()->isLeaf()) {
                batch.push_back(it.currentItem());
            }
        }
        for (size_t j = 0; j < batch.size(); j++) {
            dispatch(batch[j]);
        }
    }
    // The workflow is finished once every task in it is finished
    std::vector<Task*> leaves;
    collectLeaves(root, leaves);
    for (size_t i = 0; i < leaves.size(); i++) {
        if (!leaves[i]->isFinished()) {
            return;
        }
    }
    std::cout << "Workflow finished." << std::endl;
}

// Retries every task that timed out by sending it through the chain again
void TividyFacade::checkTimeouts() {
    if (!root) {
        std::cout << "No workflow loaded." << std::endl;
        return;
    }
    std::vector<Task*> leaves;
    collectLeaves(root, leaves);
    int retried = 0;
    for (size_t i = 0; i < leaves.size(); i++) {
        if (leaves[i]->getStateName() == "TimedOut") {
            std::cout << "Retrying timed-out task: " << leaves[i]->getName() << std::endl;
            dispatch(leaves[i]);
            retried++;
        }
    }
    if (retried == 0) {
        std::cout << "No timed-out tasks." << std::endl;
    }
}

// Sends one task down the chain. ExecuteCommand keeps a snapshot only if the task changed.
bool TividyFacade::dispatch(Task* t) {
    std::string before = t->getStateName();
    try {
        chain->handle(t, t->getDepartmentLevel());
    } catch (const std::exception& e) {
        std::cout << "Could not run " << t->getName() << ": " << e.what() << std::endl;
        return false;
    }
    if (t->getStateName() != before) {
        undoTargets.push_back(t);
        return true;
    }
    return false;
}

// ---------------- Viewing ----------------

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
    if (logQueue->empty()) {
        std::cout << "(log is empty)" << std::endl;
        return;
    }
    logQueue->flushTo(std::cout);
}

// ---------------- Lifecycle actions from the TUI ----------------

// Snapshot first, then move; an invalid move throws and the snapshot is dropped
bool TividyFacade::transition(const std::string& taskName, void (Task::*move)()) {
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
    return true;
}

bool TividyFacade::startTask(const std::string& taskName)  { return transition(taskName, &Task::start); }
bool TividyFacade::rejectTask(const std::string& taskName) { return transition(taskName, &Task::reject); }
bool TividyFacade::cancelTask(const std::string& taskName) { return transition(taskName, &Task::cancel); }
bool TividyFacade::reopenTask(const std::string& taskName) { return transition(taskName, &Task::reopen); }

// A task that has not been started yet is run through ExecuteCommand (start + complete, logged)
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
        return true;
    }
    return transition(taskName, &Task::complete);
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

// ---------------- Lookup ----------------

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
