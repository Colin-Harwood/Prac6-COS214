#include "ExecuteCommand.h"
#include "Task.h"
#include "TaskSnapshotManager.h"
#include <iostream>


ExecuteCommand::ExecuteCommand(LogQueue* q, TaskSnapshotManager* s):Command(q, s) {}

void ExecuteCommand::execute(Task* t) {
    // Prototype B: Save snapshot before any change
    snapshotManager->addSnapshot(t);

    try {
        t->start();
        t->complete();
    } catch (const std::exception& e) {
        // Undo a half-done transition
        snapshotManager->restoreLatest(t);
        record(t, "FAILED");
        throw; // Re-throw to let the handler/engine know it failed
    }

    record(t, "EXECUTED");
}


void ExecuteCommand::undo(Task* t) {
    if (snapshotManager->restoreLatest(t)) {
        record(t, "UNDONE");
    }
}


Command* ExecuteCommand::clone() const {
    ExecuteCommand* copy = new ExecuteCommand(logQueue, snapshotManager);
    copy->taskName = this->taskName;
    copy->action = this->action;
    return copy;
}
