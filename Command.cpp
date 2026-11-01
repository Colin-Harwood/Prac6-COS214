#include "Command.h"
#include "Task.h"
#include "LogQueue.h"
#include "TaskSnapshotManager.h"

Command::Command(LogQueue* q, TaskSnapshotManager* s) : logQueue(q), snapshotManager(s), taskName(""), action("") {}

std::string Command::describe() const {
    return action + ": " + taskName;
}

void Command::record(Task* t, const std::string& act) {
    this->taskName = t->getName();
    this->action = act;
    // Push a frozen, independent copy. The queue takes ownership.
    this->logQueue->push(this->clone());
}
